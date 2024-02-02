#!/usr/bin/python3

from concurrent.futures import ThreadPoolExecutor
import multiprocessing
from subprocess import run
import sys
import os
import json

def generate_input(args):
    return run(args.split(), capture_output=True).stdout.strip().decode()

def benchmark(args, input_data, benchmark_id):
    data = run(args.split(), capture_output=True, input=input_data + '\n', encoding='ascii').stdout
    return (benchmark_id, data)

def parse_program_output(data):
    success = data[1].split("\n")[4]
    benchmark_id, result = data
    if success != "success":
        return benchmark_id, { 'state': 'failed' }
    split = result.split("\n")[8].split()
    n_elements = int(split[1])
    mean_time = float(split[3])
    st_dev = float(split[6])
    moves = int(split[8])
    distance = int(split[10])
    parsed = {
        'state': 'ok',
        'nelements': n_elements,
        'mean_time': mean_time,
        'st_dev': st_dev,
        'moves': moves,
        'distance': distance
    }
    return benchmark_id, parsed

def main():
    if len(sys.argv) != 5:
        print("Usage ./{} <input length> <chance> <number of benchmark iterations> <output file name>".format(sys.argv[0]))
        sys.exit(-1)

    print("Generating input data")
    input_len = int(sys.argv[1])
    it = int(sys.argv[3])
    input_data = {}
    input_data['correct']= generate_input(f"./data-generator --correct {input_len}")
    input_data['permutation']= generate_input(f'./data-generator --random-permutation {input_len // 4} {input_len // 4} {input_len // 4} {input_len // 4}')
    input_data['random']= generate_input(f'./data-generator --total-random {input_len}')
    input_data['with_chance']= generate_input(f"./data-generator --random-with-chance {input_len} {float(sys.argv[2])}")
    print(input_data)

    astar_h = [0, 1, 2]
    naive_opt = [0, 1, 2]
    partial_bfs_arg_1 = [0, 1]
    partial_bfs_arg_2 = [1, 2]

    results = []

    with ThreadPoolExecutor(max_workers=multiprocessing.cpu_count() - 4) as pool:
        for input_data_type, input_data_str in input_data.items():

            # bfs
            results.append(pool.submit(benchmark, f"./robot-cli --bfs --iter {it}", input_data_str, f"full-bfs.{input_data_type}.{it}.{input_len}"))

            # naive
            for opt in naive_opt:
                results.append(pool.submit(benchmark, f"./robot-cli --naive {opt} --iter {it}", input_data_str, f"naive.{input_data_type}.{it}.{input_len}.{opt}"))

            # astar
            for heura in astar_h:
                results.append(pool.submit(benchmark, f"./robot-cli --astar {heura} --iter {it}", input_data_str, f"astar.{input_data_type}.{it}.{input_len}.{heura}"))

            for arg_1 in partial_bfs_arg_1:
                for arg_2 in partial_bfs_arg_2:
                    results.append(pool.submit(benchmark, f"./robot-cli --partial {arg_1} {arg_2} --iter {it}", input_data_str, f"partial.{input_data_type}.{it}.{input_len}.{arg_1}.{arg_2}"))



    results = list(map(lambda future: parse_program_output(future.result()), results))

    print("Benchmark results:")
    print(results)

    res = {
        'results': results,
        'input': input_data
    }
    with open(sys.argv[4], "w") as file:
        json.dump(res, file)

if __name__ == "__main__":
    main()
