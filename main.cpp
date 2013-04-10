#include <iostream>
#include <mpi.h>
#include <cstdlib>
#include <time.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <fstream>
#include <climits>
#include "bitonic_sort.h"


using namespace std;

void print_help()
{
    cout << "To generate random data of size 400 use ./bitonicdriver -r 400\n"
         << "To read an input file and sort use ./bitonicdriver -i filename.txt\n";
}

unsigned long long parse_input_file(char *name, vector<unsigned int> & data)
{
    ifstream ifile;
    unsigned int d;

    data.clear();
    ifile.open(name);
    if (!ifile.good())
    {
        return 0;
    }
    while(!ifile.eof())
    {
        ifile >> d;
        data.push_back(d);
    }
    return data.size();
}

void master_scatter_array(const vector<unsigned int> & data, bitonic_t * b)
{
    unsigned int i = 0;
    unsigned int * data_start;
    for (; i < b->local_data_size; ++i)
    {
        b->data[i] = data[i];
    }
    data_start = const_cast<unsigned int*>(&(data[i]));
    for (int p = 1; p < b->num_proc; ++p)
    {
        MPI_Send((void *) data_start,
                 (int) b->local_data_size,
                 MPI_INT,
                 p,
                 0,
                 MPI_COMM_WORLD);
        data_start += b->local_data_size;
    }
}

void slave_scatter_array(bitonic_t * b)
{
    MPI_Status status;
    MPI_Recv((void*) b->data,
             (int) b->local_data_size,
             MPI_INT,
             0,
             0,
             MPI_COMM_WORLD,
             &status);
}

void master_collect_array(vector<unsigned int> & data, bitonic_t * b)
{
    MPI_Status status;
    unsigned int i = 0;
    for(; i < b->local_data_size; ++i)
    {
        data[i] = b->data[i];
    }
    for(int p = 1; p < b->num_proc; ++p)
    {
        MPI_Recv((void *)b->recv_buffer,
                 (int)b->local_data_size,
                 MPI_INT,
                 p,
                 0,
                 MPI_COMM_WORLD,
                 &status);
        for(unsigned int j = 0; j < b->local_data_size; ++j, ++i)
        {
            data[i] = b->recv_buffer[j];
        }
    }
}

void slave_collect_array(bitonic_t * b)
{
    MPI_Send((void*)b->data,
             (int)b->local_data_size,
             MPI_INT,
             0,
             0,
             MPI_COMM_WORLD);
}

int main(int argc, char *argv[])
{
    bitonic_t bitonic;
    int mpi_id,
        mpi_num_procs,
        mpi_proc_name_len;
    unsigned long long global_array_size = 1000;
    char mpi_proc_name[MPI_MAX_PROCESSOR_NAME];
    bool gen_rand_data = true;
    char *input_file = NULL;
    int c;
    vector<unsigned int> global_data;

    srand(time(NULL));
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_id);
    MPI_Get_processor_name(mpi_proc_name,
                           &mpi_proc_name_len);

    while ((c = getopt(argc, argv, "r:i:")) != -1)
    {
        switch (c)
        {
            case 'r':
                gen_rand_data = true;
                global_array_size = strtoul(optarg, NULL, 0);
                if (global_array_size == 0)
                {
                    print_help();
                    exit(1);
                }
                break;
            case 'i':
                gen_rand_data = false;
                input_file = optarg;
                break;
            default:
                print_help();
                exit(1);
                break;
        };
    }
    if (optind < argc)
    {
        print_help();
        exit(1);
    }

    // Set up global array
    if (mpi_id == 0)
    {
        if (gen_rand_data)
        {
            global_data.reserve(global_array_size);
            for (unsigned int i = 0; i < global_array_size; ++i)
            {
                global_data.push_back(rand()%10);
            }
        }
        else
        {
            global_array_size = parse_input_file(input_file, global_data);
            if (global_array_size == 0)
            {
                cout << "Either the file was empty, not able to be parsed or does not exist\n";
                exit(2);
            }
        }
        // Pad global array so that its length is a
        // multiple of the number of processors
        if (global_data.size() % mpi_num_procs != 0)
        {
            global_array_size += mpi_num_procs
                                 - (global_array_size % mpi_num_procs);
            while (global_data.size() < global_array_size)
            {
                global_data.push_back(INT_MAX);
            }
        }
    }

    init_bitonic(&bitonic, mpi_id, mpi_num_procs, global_array_size);

    // // Distribute global array
    // if (mpi_id == 0)
    // {
    //     master_scatter_array(global_data, &bitonic);
    // }
    // else
    // {
    //     slave_scatter_array(&bitonic);
    // }
    // Sort
    // MPI_Barrier(MPI_COMM_WORLD);
    // bitonic_sort(&bitonic);
    // MPI_Barrier(MPI_COMM_WORLD);
    // Collect global array
    if (mpi_id == 0)
    {
        master_collect_array(global_data, &bitonic);
    }
    else
    {
        slave_collect_array(&bitonic);
    }

// if (mpi_id == 0)
// for(unsigned int i = 0; i < global_data.size(); ++i)
// {
//     cout << global_data[i] << " ";
// }

    destroy_bitonic(&bitonic);
    MPI_Finalize();
    return 0;
}
