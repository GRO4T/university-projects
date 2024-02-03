#!/bin/bash

TEST_NAME="stress_test_schwefel"
echo "[TEST] $TEST_NAME"

rm ./experiments/$TEST_NAME/*.log
rm ./experiments/$TEST_NAME/*.gif
rm ./experiments/$TEST_NAME/*.png

python3 particle_swarm_runner.py --particles 20 --obj_func schwefel --limit 500 \
								 --stop_cond max_iteration --max_iteration 200 \
								 --test --tests 1000 --omega_policy basic

python3 particle_swarm_runner.py --particles 20 --obj_func schwefel --limit 500 \
								 --stop_cond max_iteration --max_iteration 200 \
								 --test --tests 1000 --omega_policy random

python3 particle_swarm_runner.py --particles 20 --obj_func schwefel --limit 500 \
								 --stop_cond max_iteration --max_iteration 200 \
								 --test --tests 1000 --omega_policy iteration

python3 particle_swarm_runner.py --particles 20 --obj_func schwefel --limit 500 \
								 --stop_cond max_iteration --max_iteration 200 \
								 --test --tests 1000 --omega_policy global_minimum

python3 particle_swarm_runner.py --particles 20 --obj_func schwefel --limit 500 \
								 --stop_cond max_iteration --max_iteration 200 \
								 --test --tests 1000 --omega_policy iteration \
								 --c_local 2 --c_global 2

python3 particle_swarm_runner.py --particles 20 --obj_func schwefel --limit 500 \
								 --stop_cond max_iteration --max_iteration 200 \
								 --test --tests 1000 --omega_policy global_minimum \
								 --c_local 0.3 --c_global 0.1

mv logs/* 	./experiments/$TEST_NAME/
mv gifs/*	./experiments/$TEST_NAME/
mv graphs/* ./experiments/$TEST_NAME/