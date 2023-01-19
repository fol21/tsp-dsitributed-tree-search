#!/bin/bash

echo Starting Routine for UK12 Dataset [./datasets/UK12/uk12_d.txt]

echo Compiling script...
make tarefa3.mpi
echo Compilation Done.

mkdir ./logs
mkdir ./logs/UK12

for i in 1 2 4 8
do
    rm -f "./logs/UK12/log_uk12_mpi_$i_.txt"
done
    mpiexec -np 2 "./bin/tarefa3.mpi" "./datasets/UK12/uk12_d.txt" > ./logs/UK12/log_uk12_mpi_1.txt
    mpiexec -np 3 "./bin/tarefa3.mpi" "./datasets/UK12/uk12_d.txt" > ./logs/UK12/log_uk12_mpi_2.txt
    mpiexec -np 5 "./bin/tarefa3.mpi" "./datasets/UK12/uk12_d.txt" > ./logs/UK12/log_uk12_mpi_4.txt
    mpiexec -np 9 "./bin/tarefa3.mpi" "./datasets/UK12/uk12_d.txt" > ./logs/UK12/log_uk12_mpi_8.txt