#include <iostream>
#include <mpi.h>
#include "bitonic_sort.h"

using namespace std;

int main(int argc, char *argv[])
{
    bitonic_t bitonic;
    int mpi_id,
        mpi_num_procs,
        mpi_proc_name_len;
    unsigned long long global_array_size;
    char mpi_proc_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_id);
    MPI_Get_processor_name(mpi_proc_name,
                           &mpi_proc_name_len);

    ///XXX: Set size and load data
    init_bitonic(&bitonic, mpi_id, mpi_num_procs, global_array_size);
    //XXX: Record Timings
    bitonic_sort(&bitonic);

    //XXX:Collect sorted array
    //XXX:Display stuff

    MPI_Finalize();
    return 0;
}
