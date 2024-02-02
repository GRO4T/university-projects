#!/bin/bash

docker build --rm -t nginx -f ./docker/Dockerfile_nginx .
docker run -it --init -d --net=host --name nginx-serve nginx
# docker rm nginx-serve
# docker rmi nginx