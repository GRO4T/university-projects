FROM jenkins/jenkins:lts-jdk11
USER root
RUN apt update && apt install -y docker.io
COPY plugins.txt /usr/share/jenkins/ref/plugins.txt
RUN /usr/local/bin/install-plugins.sh < /usr/share/jenkins/ref/plugins.txt