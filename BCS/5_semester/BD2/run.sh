#!/bin/sh

export MARIADB_ROOT_PASSWORD=admin123
export MARIADB_USERNAME=dbuser
export MARIADB_PASSWORD=dbuser
export MARIADB_DATABASE=KorpoBaza

docker-compose build && docker-compose up
