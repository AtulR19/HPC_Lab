#!/bin/bash

# Reset results.csv and add header
echo "points,threads,pi,error,time" > results.csv

# Number of points
points=(10 100 10000 1000000 10000000 100000000 1000000000)

# Number of threads
threads=(1 2 4 8 16)

# Run every combination
for n in "${points[@]}"; do
    for t in "${threads[@]}"; do

        echo "========================================"
        echo "Running $n points with $t threads"
        echo "========================================"

        ./q1p "$n" "$t"

    done
done

echo ""
echo "========================================"
echo "All experiments completed!"
echo "Results saved in results.csv"
echo "========================================"