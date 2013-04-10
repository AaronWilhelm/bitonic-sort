///////////////////////////////////////////////////////////////////////////////////
// @file   bitonic_sort.cpp
// @author Aaron Wilhelm
// @brief  Implementation for bitonic sort algorithm using OpenMPI
///////////////////////////////////////////////////////////////////////////////////
#include "bitonic_sort.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <climits>
#include <algorithm>
#include <mpi.h>

#define EVEN(x) ((x & 1) == 0)
#define ODD(x) (!(EVEN(x)))
#define NTH_BIT(x,n) ((x>>n) & 1) // Get nth bit
#define IS_POW_2(x) (((x) & ((x)-1)) == 0)
#define MAX(a,b) ((a < b)?(b):(a))
#define MIN(a,b) ((a > b)?(b):(a))

///////////////////////////////////////////////////////////////////////////////////
// @fn lg
// @brief log base 2 approximation
// @param a - number to take lg of
// @return log base 2 of a
///////////////////////////////////////////////////////////////////////////////////
static uint32_t lg(uint32_t a)
{
    uint32_t ret = 0;

    while (a >>= 1)
    {
        ++ret;
    }
    return ret;
}

///////////////////////////////////////////////////////////////////////////////////
// @fn compare_low
// @brief This will swap lists with process i, then grab the smallest half of
//        the contents and store them in non-decreasing order locally (in b->data)
// @pre b != NULL, i < b->num_proc, the list in b->data and the recieved list
//      must be sorted in non-decreasing fashion
// @post b->data will contain the smallest b->local_data_size elements
//       of the two lists b->data and b->data of process i, and it will be
//       stored in non-decreasing order
// @param b - the bitonic data for this list
// @param i - the process to swap buffers with
///////////////////////////////////////////////////////////////////////////////////
static void compare_low(bitonic_t * b, uint32_t i)
{
    MPI_Status status;
    uint32_t i_o,
             i_r;

    MPI_Recv((void*)b->recv_buffer,
             (int32_t)b->local_data_size,
             MPI_INT,
             (int32_t) i,
             0,
             MPI_COMM_WORLD,
             &status);
    MPI_Send((void*)b->data,
             (int32_t)b->local_data_size,
             MPI_INT,
             (int32_t) i,
             0,
             MPI_COMM_WORLD);
    memcpy((void*)b->other_buffer,
           (void*)b->data,
           sizeof(b->data[0]) * b->local_data_size);
    i_o = 0;
    i_r = 0;
    for (uint64_t i = 0; i < b->local_data_size; ++i)
    {
        if (b->recv_buffer[i_r] < b->other_buffer[i_o])
        {
            b->data[i] = b->recv_buffer[i_r];
            ++i_r;
        }
        else
        {
            b->data[i] = b->other_buffer[i_o];
            ++i_o;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////
// @fn compare_high
// @brief This will swap lists with process i, then grab the largest half of
//        the contents and store them in non-decreasing order locally (in b->data)
// @pre b != NULL, i < b->num_proc, the list in b->data and the recieved list
//      must be sorted in non-decreasing fashion
// @post b->data will contain the largest b->local_data_size elements
//       of the two lists b->data and b->data of process i, and it will be
//       stored in non-decreasing order
// @param b - the bitonic data for this list
// @param i - the process to swap buffers with
///////////////////////////////////////////////////////////////////////////////////
static void compare_high(bitonic_t * b, uint32_t i)
{
    MPI_Status status;
    uint32_t i_o,
             i_r;

    MPI_Send((void*)b->data,
             (int)b->local_data_size,
             MPI_INT,
             (int32_t) i,
             0,
             MPI_COMM_WORLD);
    MPI_Recv((void*)b->recv_buffer,
             (int)b->local_data_size,
             MPI_INT,
             (int32_t) i,
             0,
             MPI_COMM_WORLD,
             &status);
    memcpy((void*)b->other_buffer,
           (void*)b->data,
           sizeof(b->data[0]) * b->local_data_size);
    i_o = b->local_data_size - 1;
    i_r = b->local_data_size - 1;
    for (uint64_t i = b->local_data_size - 1; ; --i)
    {
        if (b->recv_buffer[i_r] > b->other_buffer[i_o])
        {
            b->data[i] = b->recv_buffer[i_r];
            --i_r;
        }
        else
        {
            b->data[i] = b->other_buffer[i_o];
            --i_o;
        }
        if (i == 0)
        {
            break;
        }
    }
}

void init_bitonic(bitonic_t * b,
                  int32_t id,
                  int32_t num_proc,
                  uint64_t global_size)
{
    b->id = id;
    b->num_proc = num_proc;
    if ( ! IS_POW_2(num_proc) )
    {
        std::cerr << "The number of processes are not a power of two\n";
        exit(1);
    }
    if (global_size % num_proc != 0)
    {
        global_size += num_proc - (global_size % num_proc);
    }
    b->local_data_size = global_size / num_proc;
    b->data = new int32_t[b->local_data_size];
    for (uint64_t i = 0; i < b->local_data_size; ++i)
    {
        b->data[i] = INT_MAX;
    }
    b->recv_buffer  = new int32_t[b->local_data_size];
    b->other_buffer = new int32_t[b->local_data_size];
}

void destroy_bitonic(bitonic_t *b)
{
    delete [] b->data;
    delete [] b->recv_buffer;
    delete [] b->other_buffer;
}

uint64_t start_global_idx(bitonic_t * b)
{
    return (uint64_t)b->id * b->local_data_size;
}


void bitonic_sort(bitonic_t *b)
{
    uint32_t cube_dim = lg(b->num_proc);

    std::sort(b->data, b->data + b->local_data_size);
    MPI_Barrier(MPI_COMM_WORLD);
    for (uint32_t i = 1; i <= cube_dim; ++i)
    {
        uint32_t window_id = (uint32_t)b->id>>i;
        for (uint32_t j = i - 1; ; --j)
        {
            if ((EVEN(window_id) && NTH_BIT(b->id, j) == 0)
                || (ODD(window_id) && NTH_BIT(b->id, j) == 1))
            {
                compare_low(b, b->id ^ (0x1<<j));
            }
            else
            {
                compare_high(b, b->id ^ (0x1<<j));
            }
            if (j == 0)
            {
                break;
            }
        }
    }
}
