#!/bin/sh

export PYTHONPATH=/app

python3 /app/main.py
python3 -m pytest /test_triggers.py
