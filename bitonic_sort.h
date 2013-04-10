///////////////////////////////////////////////////////////////////////////////////
// @file   bitonic_sort.h
// @author Aaron Wilhelm
// @brief  Header for bitonic sort algorithm using OpenMPI
///////////////////////////////////////////////////////////////////////////////////
#ifndef BITONIC_SORT_H
#define BITONIC_SORT_H

#include <stdint.h>

///////////////////////////////////////////////////////////////////////////////////
// @struct bitonic_t
// @brief  Struct to use bitonic sorting algorithm
//         To use first call init_bitonic to initialize things,
//         load the local data into the variable data,
//         call bitonic_sort,
//         call destroy_bitonic
//         carry on
///////////////////////////////////////////////////////////////////////////////////
struct bitonic_t
{
    // MPI related
    int32_t id;       // MPI id number
    int32_t num_proc; // number of MPI processes

    // Data related
    uint64_t local_data_size; // Size of local data
    int32_t * data;           // Data for distributed array

    // Used internally as buffers for holding temporary data
    int32_t * recv_buffer;
    int32_t * other_buffer;
};

///////////////////////////////////////////////////////////////////////////////////
// @fn init_bitonic
// @brief Initialize the structure for bitonic sorting
// @pre b != NULL, id >= 0 && id < num_proc, global_size >= num_proc
// @post b is initialized
// @param b - bitonic_t structure to initialize
// @param id - ID number of this process
// @param num_proc - Total number of processes
// @param global_size - Size of the global array
///////////////////////////////////////////////////////////////////////////////////
void init_bitonic(bitonic_t * b,
                  int32_t id,
                  int32_t num_proc,
                  uint64_t global_size);

///////////////////////////////////////////////////////////////////////////////////
// @fn destroy_bitonic
// @brief Deallocates, memory in the struct
// @pre b != NULL, init_bitonic(b), must have been called first
///////////////////////////////////////////////////////////////////////////////////
void destroy_bitonic(bitonic_t *b);

///////////////////////////////////////////////////////////////////////////////////
// @fn start_global_idx
// @brief Index into global array associated with
//        first data point in the segment in b
// @Note: The the global array doesn't necessarily exist
// @param b - bitonic sort data
// @return Index into global array associated with
//        first data point in the segment in b
///////////////////////////////////////////////////////////////////////////////////
inline uint64_t start_global_idx(bitonic_t *b);

///////////////////////////////////////////////////////////////////////////////////
// @fn bitonic_sort
// @brief 
// @pre b must be initialized correctly (using init_bitonic)
// @post the data in b will be sorted and apart of a global sorted array
// @param b -  the part of the distrubuted array to sort
///////////////////////////////////////////////////////////////////////////////////
void bitonic_sort(bitonic_t *b);

#endif /* BITONIC_SORT_H */
