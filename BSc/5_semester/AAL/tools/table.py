#!/usr/bin/python3

import os
import sys
import json
from math import *

INPUT_FILE='out2.txt'

def split_by_algorithm(results):
    algorithms = ['full-bfs', 'partial', 'naive', 'astar']

    # print("res: ", results)
    return { name : list(filter(lambda benchmark_res: benchmark_res[0].startswith(name), results)) for name in algorithms }

def T(n):
    return n**n**0.69

def main():
    with open(INPUT_FILE, "r") as file:
        res = json.loads(file.read())

    results = res['results']

    bfs = split_by_algorithm(results)['full-bfs']

    times= {}
    n = {}
    for entry in bfs:
        if entry[1]['nelements'] not in times:
            times[entry[1]['nelements']]= 0
        if entry[1]['nelements'] not in n:
            n[entry[1]['nelements']]= 0

        times[entry[1]['nelements']] += entry[1]['mean_time']
        n[entry[1]['nelements']] += 1


    norm = {}
    for ind, time in times.items():
        norm[ind] = time / n[ind]

    c= T(9) / norm[9]

    for i in range(6, 13):
        print((norm[i]/T(i))*c)

    print("\n\n\n")

    t = []
    for i in range(6, 13):
        t.append(log(log(norm[i], i), i))

    print(sum(t) / len(t))



if __name__ == "__main__":
    main()
