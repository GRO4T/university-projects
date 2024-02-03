"""
Author: Rafał Kulus
"""

import subprocess
import threading
from time import sleep

RUNS = 256
# DATA_SET = "data/students/student-mat.csv"
# CLASS = "Dalc"
DATA_SET = "./data/divorce.csv"
CLASS = "Class"
# VENV = "./venv/Scripts/python.exe" # Windows
VENV = "./venv/bin/python3" # Linux
CONCURRENT_JOBS = 6
UPPER = 6

current = 0
running = 0
all = 0
jobs = []


def run_process(job):
    global running
    proc = subprocess.Popen(job, shell=True)
    proc.wait()
    running -= 1


if __name__ == '__main__':
    for test in range(1, UPPER):
        for prune in range(1, UPPER):
            jobs.append(f"{VENV} ./app/main.py {DATA_SET} {CLASS} {test/10} {prune/10} {RUNS}")
            all += 1

    while current != all or running > 0:
        print(f"All: {all} / Current: {current} / Running: {running}")
        if current != all and running < CONCURRENT_JOBS:
            running += 1
            print(jobs[current])
            thread = threading.Thread(target=run_process, args=(jobs[current],))
            thread.start()
            current += 1
        else:
            sleep(1)
