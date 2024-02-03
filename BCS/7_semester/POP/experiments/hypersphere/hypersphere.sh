#!/bin/bash

TEST_NAME="hypersphere"
echo "[TEST] $TEST_NAME"

rm ./experiments/$TEST_NAME/*.log
rm ./experiments/$TEST_NAME/*.gif
rm ./experiments/$TEST_NAME/*.png

python3 particle_swarm_runner.py --particles 20 --obj_func hypersphere --limit 10 \
								 --stop_cond max_iteration --max_iteration 100 \
								 --graph --graph_omega --omega_policy basic \

python3 particle_swarm_runner.py --particles 20 --obj_func hypersphere --limit 10 \
								 --stop_cond max_iteration --max_iteration 100 \
								 --graph --graph_omega --omega_policy random 

python3 particle_swarm_runner.py --particles 20 --obj_func hypersphere --limit 10 \
								 --stop_cond max_iteration --max_iteration 100 \
								 --graph --graph_omega --omega_policy iteration

python3 particle_swarm_runner.py --particles 20 --obj_func hypersphere --limit 10 \
								 --stop_cond max_iteration --max_iteration 100 \
								 --graph --graph_omega --omega_policy global_minimum

mv logs/* 	./experiments/$TEST_NAME/
mv gifs/*	./experiments/$TEST_NAME/
mv graphs/* ./experiments/$TEST_NAME/