#!/bin/bash
echo "`date +%y/%m/%d_%H:%M:%S`:: Starting random func 1"
bash test_parallelism_random.sh 1 | tee -a test_parallelism.log | grep '"exec_time"' | awk '{ print $2 }' | python3 create_test_parallelism_plots.py random 1
echo "`date +%y/%m/%d_%H:%M:%S`:: Random func 1 completed"

echo "`date +%y/%m/%d_%H:%M:%S`:: Starting random func 2"
bash test_parallelism_random.sh 2 | tee -a test_parallelism.log | grep '"exec_time"' | awk '{ print $2 }' | python3 create_test_parallelism_plots.py random 2
echo "`date +%y/%m/%d_%H:%M:%S`:: Random func 2 completed"

echo "`date +%y/%m/%d_%H:%M:%S`:: Starting swarm func 1"
bash test_parallelism_swarm.sh 1 | tee -a test_parallelism.log | grep '"exec_time"' | awk '{ print $2 }' | python3 create_test_parallelism_plots.py swarm 1
echo "`date +%y/%m/%d_%H:%M:%S`:: Swarm func 1 completed"

echo "`date +%y/%m/%d_%H:%M:%S`:: Starting swarm func 2"
bash test_parallelism_swarm.sh 2 | tee -a test_parallelism.log | grep '"exec_time"' | awk '{ print $2 }' | python3 create_test_parallelism_plots.py swarm 2
echo "`date +%y/%m/%d_%H:%M:%S`:: Swarm func 2 completed"
