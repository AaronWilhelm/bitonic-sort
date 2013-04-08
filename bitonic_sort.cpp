#include "bitonic_sort.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <climits>

#define IS_POW_2(x) (((x) & ((x)-1)) == 0)

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
}

void destroy_bitonic(bitonic_t *b)
{
    delete [] b->data;
}

uint64_t start_global_idx(bitonic_t *b)
{
    return (uint64_t)b->id * b->local_data_size;
}

void bitonic_sort(bitonic_t *b)
{
    ///XXX: Write
}
