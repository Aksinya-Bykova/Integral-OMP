#!/bin/bash

cd /home/xunya/CLionProjects/omp-custom/cmake-build-debug/

cmake ..

make

for i in {1..100}; do
#for i in {1..1}; do
#    echo "Run #$i"
    ./omp_custom --omp-threads 6 --input /home/xunya/CLionProjects/omp-custom/research/custom-input.txt --output /home/xunya/CLionProjects/omp-custom/research/custom-output.txt
done

