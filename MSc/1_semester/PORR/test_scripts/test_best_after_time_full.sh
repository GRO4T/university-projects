#!/bin/bash

REPEAT=10

for time_s in 5 10 15
do
  echo "`date +%y/%m/%d_%H:%M:%S`:: Starting random func 1 - time limit: ${time_s} s"
  bash test_best_after_time_random.sh 1 $time_s $REPEAT | grep '"value"' | awk '{ print $2 }' | python3 create_test_best_after_time_plots.py random 1 $time_s $REPEAT
  echo "`date +%y/%m/%d_%H:%M:%S`:: Random func 1 completed"

  echo "`date +%y/%m/%d_%H:%M:%S`:: Starting random func 2 - time limit: ${time_s} s"
  bash test_best_after_time_random.sh 2 $time_s $REPEAT | grep '"value"' | awk '{ print $2 }' | python3 create_test_best_after_time_plots.py random 2 $time_s $REPEAT
  echo "`date +%y/%m/%d_%H:%M:%S`:: Random func 2 completed"

  echo "`date +%y/%m/%d_%H:%M:%S`:: Starting swarm func 1 - time limit: ${time_s} s"
  bash test_best_after_time_swarm.sh 1 $time_s $REPEAT | grep '"value"' | awk '{ print $2 }' | python3 create_test_best_after_time_plots.py swarm 1 $time_s $REPEAT
  echo "`date +%y/%m/%d_%H:%M:%S`:: Swarm func 1 completed"

  echo "`date +%y/%m/%d_%H:%M:%S`:: Starting swarm func 2 - time limit: ${time_s} s"
  bash test_best_after_time_swarm.sh 2 $time_s $REPEAT | grep '"value"' | awk '{ print $2 }' | python3 create_test_best_after_time_plots.py swarm 2 $time_s $REPEAT
  echo "`date +%y/%m/%d_%H:%M:%S`:: Swarm func 2 completed"
done