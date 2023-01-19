#!/bin/bash

echo Starting Routine for P01 Dataset [./datasets/P01/uk12_d.txt]

echo Compiling script...
make tarefa3
echo Compilation Done.

mkdir ./logs
mkdir ./logs/P01

for i in 1 2 4 8
do
    rm -f "./logs/P01/log_p01_pth_$i_.txt"
done
    "./bin/tarefa3" 1 "./datasets/P01/p01_d.txt" > ./logs/P01/log_p01_pth_1.txt
    "./bin/tarefa3" 2 "./datasets/P01/p01_d.txt" > ./logs/P01/log_p01_pth_2.txt
    "./bin/tarefa3" 4 "./datasets/P01/p01_d.txt" > ./logs/P01/log_p01_pth_4.txt
    "./bin/tarefa3" 8 "./datasets/P01/p01_d.txt" > ./logs/P01/log_p01_pth_8.txt