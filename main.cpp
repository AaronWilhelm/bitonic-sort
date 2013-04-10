#include <iostream>
#include <mpi.h>
#include <cstdlib>
#include <time.h>
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
    bool gen_rand_data;

    srand(time(NULL));
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_id);
    MPI_Get_processor_name(mpi_proc_name,
                           &mpi_proc_name_len);

    cout << "This is process " << mpi_id << " of " << mpi_num_procs << endl;

    ///XXX: Set size and load data
    gen_rand_data = true;
    if (gen_rand_data)
    {
        global_array_size = 20;
        init_bitonic(&bitonic, mpi_id, mpi_num_procs, global_array_size);
        for (unsigned int i = 0; i < bitonic.local_data_size; ++i)
        {
            bitonic.data[i] = i + mpi_id;
        }
        bitonic_sort(&bitonic);

        for(int i = 0; i < mpi_num_procs; ++i)
        {
            MPI_Barrier(MPI_COMM_WORLD);
            if (i == mpi_id)
            {
                cout << "id = " << mpi_id << endl;
                for(unsigned int d = 0; d < bitonic.local_data_size; ++d)
                {
                    cout << mpi_id << ": " << bitonic.data[d] << "  ";
                }
            }

        }
        MPI_Barrier(MPI_COMM_WORLD);
    }


    //XXX: Record Timings
    // bitonic_sort(&bitonic);

    //XXX:Collect sorted array
    //XXX:Display stuff

    MPI_Finalize();
    return 0;
}
