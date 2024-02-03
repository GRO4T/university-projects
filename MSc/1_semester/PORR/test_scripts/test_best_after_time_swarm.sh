#!/bin/bash

objective_func_id=$1
time_limit=$2
repeat=$3

cd $(git rev-parse --show-toplevel) && cd build

for ((i=1; i <= repeat; i++))
do
  for n_dim in 20 50 100 200
  do
    ./runner -n $n_dim -s swarm -f "$objective_func_id" --time "$time_limit" -p 100
  done
done
