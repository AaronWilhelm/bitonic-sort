#!/bin/bash
changecom(`/*', `*/')
## Specify a jobname
#PBS -N name

## Set the wall time HH:MM:SS (jobs using <=15 minutes are given priority)
#PBS -l walltime=0:15:00

## Set The number of nodes (jobs using <=16 nodes with <=2 ppn are given priority)
#PBS -l nodes=num_proc

## Allocate some memory at each processor
##PBS -l mem=4gb

## Combine standard output and error files
#PBS -j oe

## Set the submission queue
#PBS -q @nic-cluster.mst.edu

## Prerun that writes useful job info into the error file
/share/apps/job_data_prerun.py

mpirun -n num_proc  /nethome/users/ajw7v3/CS387/bitonic-sort/bitonicdriver -r size
