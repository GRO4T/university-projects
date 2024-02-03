#!/usr/bin/python

import json
import sys
import os
import matplotlib.pyplot as plt

COLORS = ['b', 'g', 'r', 'c', 'm', 'y', 'k']
PARTIA_BFS_OMIT = []
ASTAR_OMIT = []

def split_by_algorithm(results):
    algorithms = ['full-bfs', 'partial', 'naive', 'astar']

    # print("res: ", results)
    return { name : list(filter(lambda benchmark_res: benchmark_res[0].startswith(name), results)) for name in algorithms }

def fix_err(err):
    return list(map(lambda x: x if x > 0 else 0, err))

def get_arg(name, results):
    return list(map(lambda benchmark_res: benchmark_res[1][name], results))

def get_plot_args(results):
    return get_arg("input_len", results), get_arg("mean_time", results), get_arg("st_dev", results)

def fix_st_dev(st_dev):
    return st_dev if st_dev > 0.0 else 0.0

def plot_full_bfs(results):
    x, y, err = get_plot_args(results)
    err = fix_err(err)
    print(x, y, err)
    plt.scatter(x=x, y=y,  color='red', label='full bfs')

def plot_partial_bfs(results):
    it = 0
    args = {}
    for result in results:
        _, _, _, _, arg1, arg2 = result[0].split(".")
        if (arg1, arg2) not in args:
            args[(arg1, arg2)] = COLORS[it]
            legend = f"partial bfs {arg1} {arg2}"
            it = it + 1
        else:
            legend = None
        result = result[1]
        x, y, err = result['input_len'], result['mean_time'], fix_st_dev(result['st_dev'])
        print(x, y, err)
        plt.errorbar(x=[x], y=[y], xerr=[err], fmt='s', color=args[(arg1, arg2)], label=legend)

def plot_astar(results):
    it = 0
    args = {}
    for result in results:
        _, _, _, _, heura = result[0].split(".")
        if heura not in ASTAR_OMIT:
            if heura not in args:
                args[heura] = COLORS[it]
                it = it + 1
                legend = f"astar {heura}"
            else:
                legend = None
            result = result[1]
            x, y, err = result['input_len'], result['mean_time'], fix_st_dev(result['st_dev'])
            print(x, y, err)
            plt.errorbar(x=[x], y=[y], xerr=[err], fmt='*', color=args[heura], label=legend)

def plot_naive(results):
    it = 0
    args = {}
    for result in results:
        _, _, _, _, opt = result[0].split(".")
        if opt not in args:
            args[opt] = COLORS[it]
            legend=f"naive {opt}"
            it = it + 1
        else:
            legend= None
        result = result[1]
        x, y, err = result['input_len'], result['mean_time'], fix_st_dev(result['st_dev'])
        print(x, y, err)
        plt.errorbar(x=[x], y=[y], xerr=[err], fmt='x', color=args[opt], label=legend)

def make_plot(func, results, input_type, param, input_str):
    plt.clf()
    plt.figure(figsize=(8.25, 11.75 / 2))
    data_to_plot = split_by_algorithm(list(filter(lambda benchmark_res: input_type in benchmark_res[0], results)))
    # print(data_to_plot)
    func(data_to_plot[param])
    # plot_full_bfs(data_to_plot['full-bfs'])
    # plot_partial_bfs(data_to_plot['partial'])
    # plot_astar(data_to_plot['astar'])
    # plot_naive(data_to_plot['naive'])
    plt.xlabel('Długość wejścia')
    plt.ylabel('Czas wykonania w us')
    plt.title(f"Algorytm {param}")
    plt.legend()
    plt.savefig(f"figs/{param}.png")


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
        # print(input_type)
        # plt.savefig(f"figs/{input_str}.png")
        make_plot(plot_full_bfs, results, input_type, 'full-bfs', input_str)
        make_plot(plot_partial_bfs, results, input_type, 'partial', input_str)
        make_plot(plot_naive, results, input_type, 'naive', input_str)
        make_plot(plot_astar, results, input_type, 'astar', input_str)




if __name__ == "__main__":
    main()
