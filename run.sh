#!/bin/bash

set -x

\rm *.o *.x vec1.txt vec2.txt

cc -c -g qsort.c
g++ -g -std=c++11 main.cc qsort.o -o main.x

./main.x 1000000

diff vec1.txt vec2.txt
