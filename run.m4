#!/bin/bash
changecom(`/*', `*/')
/* ***********************************************************************************
   This file is part of bitonic-sort

   bitonic-sort is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

    bitonic-sort is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with bitonic-sort.  If not, see <http://www.gnu.org/licenses/>.
  
    Copyright 2013 Aaron Wilhelm
************************************************************************************* */

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
