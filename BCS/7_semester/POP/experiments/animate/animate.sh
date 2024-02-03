#!/bin/bash

TEST_NAME="animate"
echo "[TEST] $TEST_NAME"

if [ $# -gt 0 ] && [ $1 = "quick" ]; then
	echo "skipping"
	exit 0
fi

rm ./experiments/$TEST_NAME/*.log
rm ./experiments/$TEST_NAME/*.gif
rm ./experiments/$TEST_NAME/*.png


python3 particle_swarm_runner.py --particles 20 --obj_func easom --limit 100 \
								 --gif 100 100 --frames 150 --omega_policy basic \
								 --c_local 2.05 --c_global 2.05

python3 particle_swarm_runner.py --particles 20 --obj_func schwefel --limit 500 \
								 --gif 500 500 --frames 150 --omega_policy basic \
								 --c_local 2.05 --c_global 2.05


mv logs/* 	./experiments/$TEST_NAME/
mv gifs/*	./experiments/$TEST_NAME/
mv graphs/* ./experiments/$TEST_NAME/