#!/bin/bash

echo "threads,time" > results2.csv

N=4000

for threads in 1 2 4 8 16
do
    echo "Running $threads threads..."

    ./q2p $N $threads

done