#include "bitonic_sort.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <climits>
#include <algorithm>
#include <mpi.h>

#define EVEN(x) ((x & 1) == 0)
#define ODD(x) (!(EVEN(x)))
#define NTH_BIT(x,n) ((x>>n) & 1)
#define IS_POW_2(x) (((x) & ((x)-1)) == 0)
#define MAX(a,b) ((a < b)?(b):(a))
#define MIN(a,b) ((a > b)?(b):(a))

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
    b->buffer = new int32_t[b->local_data_size];
}

void destroy_bitonic(bitonic_t *b)
{
    delete [] b->data;
    delete [] b->buffer;
}

uint64_t start_global_idx(bitonic_t * b)
{
    return (uint64_t)b->id * b->local_data_size;
}

void compare_low(bitonic_t * b, uint32_t i)
{
    MPI_Status status;

    MPI_Recv((void*)b->buffer,
             (int)b->local_data_size,
             MPI_INT,
             (int32_t) i,
             0,
             MPI_COMM_WORLD,
             &status);
    MPI_Send((void*)b->data,
             (int)b->local_data_size,
             MPI_INT,
             (int32_t) i,
             0,
             MPI_COMM_WORLD);
    for (uint64_t i = 0; i < b->local_data_size; ++i)
    {
        b->data[i] = MIN(b->data[i], b->buffer[i]);
    }
}

void compare_high(bitonic_t * b, uint32_t i)
{
    MPI_Status status;

    MPI_Send((void*)b->data,
             (int)b->local_data_size,
             MPI_INT,
             (int32_t) i,
             0,
             MPI_COMM_WORLD);
    MPI_Recv((void*)b->buffer,
             (int)b->local_data_size,
             MPI_INT,
             (int32_t) i,
             0,
             MPI_COMM_WORLD,
             &status);
    for (uint64_t i = 0; i < b->local_data_size; ++i)
    {
        b->data[i] = MAX(b->data[i], b->buffer[i]);
    }
}

void bitonic_sort(bitonic_t *b)
{
    uint32_t cube_dim = b->num_proc >> 1; // lg(b->num_proc)
    std::sort(b->data, b->data + b->local_data_size);

    for (uint32_t i = 0; i < cube_dim; ++i)
    {
        uint32_t window_id = cube_dim>>i;
        for (uint32_t j = i - 1; ; --j)
        {
            if ((EVEN(window_id) && NTH_BIT(b->num_proc, j) == 0)
                || (ODD(window_id) && NTH_BIT(b->num_proc, j) == 1))
            {
                // compare_low(b, j);
            }
            else
            {
                // compare_high(b, j);
            }
            if (j == 0)
            {
                break;
            }
        }
    }
}
