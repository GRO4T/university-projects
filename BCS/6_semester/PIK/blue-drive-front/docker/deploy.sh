#!/bin/bash

docker build --rm -t gatsby -f ./docker/Dockerfile .
docker run -it --init -d -p 8090:8090 --name gatsby-serve gatsby