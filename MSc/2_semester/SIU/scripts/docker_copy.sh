#!/bin/bash

CONTAINER_NAME=$1

docker cp data/roads2.png $CONTAINER_NAME:/roads.png
docker cp data/routes2.csv $CONTAINER_NAME:/root/routes.csv
docker cp solutions/turtlesim_env_base.py $CONTAINER_NAME:/root/
docker cp solutions/turtlesim_env_single.py $CONTAINER_NAME:/root/
docker cp solutions/dqn_single.py $CONTAINER_NAME:/root/
docker cp solutions/test_model.py $CONTAINER_NAME:/root/
docker cp scripts/docker_setup.sh $CONTAINER_NAME:/root/
docker cp solutions/dqn_multi.py $CONTAINER_NAME:/root/
docker cp solutions/turtlesim_env_multi.py $CONTAINER_NAME:/root/
docker cp solutions/play-dqn.py $CONTAINER_NAME:/root/
docker cp solutions/logger.py $CONTAINER_NAME:/root/
docker cp models $CONTAINER_NAME:/root/
docker cp solutions/play_dqn.sh $CONTAINER_NAME:/root/