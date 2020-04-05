#!/bin/bash

set -x

\rm *.o *.x vec1.txt vec2.txt

cc -c -O2 -g qsort.c
g++ -g -O2  -std=c++11 main.cc qsort.o -o main.x


if [ "x$1" = "x" ]
then
  n=1000000
else
  n=$1
fi

#perf record \
./main.x $n

diff vec1.txt vec2.txt
