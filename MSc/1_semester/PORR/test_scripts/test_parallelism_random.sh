#!/bin/bash

objective_func_id=$1

ITERATIONS=1000000
DIMENSION=50

cd $(git rev-parse --show-toplevel) && cd build
./runner -n $DIMENSION -i $ITERATIONS -s random -f $objective_func_id -t 1
./runner -n $DIMENSION -i $ITERATIONS -s random -f $objective_func_id -t 2
./runner -n $DIMENSION -i $ITERATIONS -s random -f $objective_func_id -t 3
./runner -n $DIMENSION -i $ITERATIONS -s random -f $objective_func_id -t 4
./runner -n $DIMENSION -i $ITERATIONS -s random -f $objective_func_id -t 5
./runner -n $DIMENSION -i $ITERATIONS -s random -f $objective_func_id -t 6
./runner -n $DIMENSION -i $ITERATIONS -s random -f $objective_func_id -t 7
./runner -n $DIMENSION -i $ITERATIONS -s random -f $objective_func_id -t 8
./runner -n $DIMENSION -i $ITERATIONS -s random -f $objective_func_id -t 9
./runner -n $DIMENSION -i $ITERATIONS -s random -f $objective_func_id -t 10
./runner -n $DIMENSION -i $ITERATIONS -s random -f $objective_func_id -t 11
./runner -n $DIMENSION -i $ITERATIONS -s random -f $objective_func_id -t 12
./runner -n $DIMENSION -i $ITERATIONS -s random -f $objective_func_id -t 13
./runner -n $DIMENSION -i $ITERATIONS -s random -f $objective_func_id -t 14
./runner -n $DIMENSION -i $ITERATIONS -s random -f $objective_func_id -t 15
./runner -n $DIMENSION -i $ITERATIONS -s random -f $objective_func_id -t 16