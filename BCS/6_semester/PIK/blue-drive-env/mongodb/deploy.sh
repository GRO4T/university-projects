docker run -d --name mongo -p 8097:27017 \
  -v /docker_volumes/mongo:/data/db \
  mvertes/alpine-mongo