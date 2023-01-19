#!/bin/bash

echo Starting Routine for P01 Dataset [./datasets/P01/uk12_d.txt]

echo Compiling script...
make tarefa3.mpi
echo Compilation Done.

mkdir ./logs
mkdir ./logs/P01

for i in 1 2 4 8
do
    rm -f "./logs/P01/log_p01_mpi_$i_.txt"
done
    mpiexec -np 2 "./bin/tarefa3.mpi" "./datasets/P01/p01_d.txt" > ./logs/P01/log_p01_mpi_1.txt
    mpiexec -np 3 "./bin/tarefa3.mpi" "./datasets/P01/p01_d.txt" > ./logs/P01/log_p01_mpi_2.txt
    mpiexec -np 5 "./bin/tarefa3.mpi" "./datasets/P01/p01_d.txt" > ./logs/P01/log_p01_mpi_4.txt
    mpiexec -np 9 "./bin/tarefa3.mpi" "./datasets/P01/p01_d.txt" > ./logs/P01/log_p01_mpi_8.txt