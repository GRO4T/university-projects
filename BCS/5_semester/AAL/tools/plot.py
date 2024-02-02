#!/usr/bin/python

import json
import sys
import os
import matplotlib.pyplot as plt

COLORS = ['b', 'g', 'r', 'c', 'm', 'y', 'k']
PARTIA_BFS_OMIT = []
ASTAR_OMIT = ['0','1']

def split_by_algorithm(results):
    algorithms = ['full-bfs', 'partial', 'naive', 'astar']

    return { name : list(filter(lambda benchmark_res: benchmark_res[0].startswith(name), results)) for name in algorithms }

def get_arg(name, results):
    return list(map(lambda benchmark_res: benchmark_res[1][name], results))

def get_plot_args(results):
    return get_arg("mean_time", results), get_arg("moves", results), get_arg("st_dev", results)

def fix_st_dev(st_dev):
    return st_dev if st_dev > 0.0 else 0.0

def plot_full_bfs(results):
    x, y, err = get_plot_args(results)
    print(x, y, err)
    plt.errorbar(x=x, y=y, xerr=err, fmt='o', color='red', label='full bfs')

def plot_partial_bfs(results):
    it = 0
    for result in results:
        _, _, _, _, arg1, arg2 = result[0].split(".")
        result = result[1]
        x, y, err = result['mean_time'], result['moves'], fix_st_dev(result['st_dev'])
        print(x, y, err)
        plt.errorbar(x=[x], y=[y], xerr=[err], fmt='s', color=COLORS[it], label=f"partial bfs {arg1} {arg2}")
        it = it + 1

def plot_astar(results):
    it = 0
    for result in results:
        _, _, _, _, heura = result[0].split(".")
        if heura not in ASTAR_OMIT:
            result = result[1]
            x, y, err = result['mean_time'], result['moves'], fix_st_dev(result['st_dev'])
            print(x, y, err)
            plt.errorbar(x=[x], y=[y], xerr=[err], fmt='*', color=COLORS[it], label=f"astar {heura}")
            it = it + 1

def plot_naive(results):
    it = 0
    for result in results:
        _, _, _, _, opt = result[0].split(".")
        result = result[1]
        x, y, err = result['mean_time'], result['moves'], fix_st_dev(result['st_dev'])
        print(x, y, err)
        plt.errorbar(x=[x], y=[y], xerr=[err], fmt='x', color=COLORS[it], label=f"naive {opt}")
        it = it + 1

def main():
    if len(sys.argv) != 2:
        print(f"Usage ./{sys.argv[0]} <input file>")
        sys.exit(-1)

    with open(sys.argv[1], "r") as file:
        res = json.load(file)


    results = res['results']
    inputs = res['input']

    for input_type, input_str in inputs.items():
        if input_type == "correct":
            continue
        plt.clf()
        plt.figure(figsize=(8.25, 11.75 / 2))
        data_to_plot = split_by_algorithm(list(filter(lambda benchmark_res: input_type in benchmark_res[0], results)))
        plot_full_bfs(data_to_plot['full-bfs'])
        plot_partial_bfs(data_to_plot['partial'])
        plot_astar(data_to_plot['astar'])
        plot_naive(data_to_plot['naive'])
        plt.xlabel('Czas wykonania w us')
        plt.ylabel('Ilość ruchów')
        plt.title(f"Dla {input_str}")
        plt.legend()
        plt.savefig(f"figs/{input_str}.png")




if __name__ == "__main__":
    main()
