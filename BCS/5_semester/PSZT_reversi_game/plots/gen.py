import subprocess
import threading
from time import sleep

current = 0
running = 0
all = 0
jobs = []


def run_process(job):
    global running
    proc = subprocess.Popen(job)
    proc.wait()
    running -= 1


for x in range(1, 7):
    for y in range(1, 7):
        jobs.append(f"python.exe ../plot_gen.py {x} {y} y n")
        all += 1

for x in range(7, 9):
    for y in range(1, 7):
        jobs.append(f"python.exe ../plot_gen.py {x} {y} n n")
        all += 1

for x in range(1, 7):
    for y in range(7, 9):
        jobs.append(f"python.exe ../plot_gen.py {x} {y} n n")
        all += 1

while current != all or running > 0:
    print(f"All: {all} / Current: {current} / Running: {running}")
    if current != all and running < 6:
        running += 1
        print(jobs[current])
        thread = threading.Thread(target=run_process, args=(jobs[current],))
        thread.start()
        current += 1
    else:
        sleep(1)
