#!/bin/bash

echo Starting Routine for UK12 Dataset [./datasets/UK12/uk12_d.txt]

echo Compiling script...
make tarefa3
echo Compilation Done.

mkdir ./logs
mkdir ./logs/UK12

for i in 1 2 4 8
do
    rm -f "./logs/UK12/log_uk12_pth_$i_.txt"
done

"./bin/tarefa3" 1 "./datasets/UK12/uk12_d.txt" > ./logs/UK12/log_uk12_pth_1.txt
"./bin/tarefa3" 2 "./datasets/UK12/uk12_d.txt" > ./logs/UK12/log_uk12_pth_2.txt
"./bin/tarefa3" 4 "./datasets/UK12/uk12_d.txt" > ./logs/UK12/log_uk12_pth_4.txt
"./bin/tarefa3" 8 "./datasets/UK12/uk12_d.txt" > ./logs/UK12/log_uk12_pth_8.txt


for i in 1 2 4 8
do
    cat "./logs/UK12/log_uk12_pth_$i.txt" | grep "Total calculation time:"
done

