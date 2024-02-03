#!/bin/bash

MODEL=$1
N_AGENTS=$2
N_STEPS=$3

CMD="python3 play-dqn.py --multi --detect_collision --n_agents=$N_AGENTS --n_steps=$N_STEPS --trace_file='trace.pkl' $MODEL" 
echo "Running test scenario: $CMD"
eval $CMD
