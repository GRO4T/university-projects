#!/usr/bin/python3

import os
import sys
import json
from math import *

INPUT_FILE='out3.txt'

def split_by_algorithm(results):
    algorithms = ['full-bfs', 'partial', 'naive', 'astar']

    # print("res: ", results)
    return { name : list(filter(lambda benchmark_res: benchmark_res[0].startswith(name), results)) for name in algorithms }

def T(n):
    return n**2

def main():
    with open(INPUT_FILE, "r") as file:
        res = json.loads(file.read())

    results = res['results']

    naive = split_by_algorithm(results)['naive']

    times= {}
    n = {}
    for entry in naive:
        _, _, _, _, opt = entry[0].split(".")
        if opt not in '0':
            continue
            pass
        if entry[1]['nelements'] not in times:
            times[entry[1]['nelements']]= 0
        if entry[1]['nelements'] not in n:
            n[entry[1]['nelements']]= 0

        times[entry[1]['nelements']] += entry[1]['mean_time']
        n[entry[1]['nelements']] += 1

    print(times)

    norm = {}
    for ind, time in times.items():
        norm[ind] = time / n[ind]

    c= T(75) / norm[75]

    for i in range(5, 176, 10):
        if i in norm:
            print((norm[i]/T(i))*c)



if __name__ == "__main__":
    main()
