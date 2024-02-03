#!/bin/bash
wget https://bootstrap.pypa.io/get-pip.py
python3 get-pip.py
wget https://launchpad.net/~ubuntu-security-proposed/+archive/ubuntu/ppa/+build/24866409/+files/python3.8-venv_3.8.10-0ubuntu1~20.04.6_amd64.deb
apt install ./python3.8-venv_3.8.10-0ubuntu1~20.04.6_amd64.deb
python3 -m venv --system-site-packages ./siu_venv
source ./siu_venv/bin/activate
pip install keras tensorflow