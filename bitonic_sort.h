#ifndef BITONIC_SORT_H
#define BITONIC_SORT_H

#include <stdint.h>

struct bitonic_t
{
    // MPI related
    int32_t id;       // MPI id number
    int32_t num_proc; // number of MPI processes

    // Data related
    uint64_t local_data_size;
    int32_t * data;
};

void init_bitonic(bitonic_t * b,
                  int32_t id,
                  int32_t num_proc,
                  uint64_t global_size);

void destroy_bitonic(bitonic_t *b);

// Index into global array associated with
// first data point in the segment in b
// @Note: The the global array doesn't necessarily exist
inline uint64_t start_global_idx(bitonic_t *b);

void bitonic_sort(bitonic_t *b);

#endif /* BITONIC_SORT_H */
