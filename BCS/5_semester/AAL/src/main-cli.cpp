#include <boost/program_options.hpp>
#include <functional>
#include <inc/algorithm/benchmark.hpp>
#include <iomanip>
#include <iostream>

#include "algorithm/generator.hpp"
#include "inc/algorithm/algorithm.hpp"

namespace po = boost::program_options;

int calculateDistance(const int len, const std::vector<std::size_t>& moves) {
    int dist = 0;
    for (const auto& m : moves) {
        dist += len - 4 - m;
    }
    return dist;
}

template <typename Fun>
void benchmark(Fun&& fun, const std::string& data, int num_iterations = 1) {
    Benchmark<> benchmark(num_iterations);
    PropertyParser result = benchmark.benchmark(fun, data)[0];

    if (result.get_success() == "Success") {
        std::cout << "success\n";
        std::cout << "iterations: " << num_iterations << std::endl;
        std::cout << "result: " << result.get_sorted_string() << std::endl;
        std::cout << "time in microseconds" << std::endl;
        std::cout << "#elements: " << std::setw(9) << data.length() << " "
                  << " mean: " << std::setw(8) << benchmark.mean()
                  << " st. dev: " << std::setw(8)
                  << benchmark.standard_deviation()
                  << " moves: " << std::setw(8)
                  << result.get_moves().size()
                  << " distance: " << std::setw(8)
                  << calculateDistance(data.length(), result.get_moves())
                  << std::endl;
    } else {
        std::cout << "fail\n";
        std::cout << result.get_fail_reson() << "\n";
    }
}

int main(int argc, char* argv[]) {
    using namespace std;
    po::options_description desc("Usage: robot-cli <algorithm> [options]");
    desc.add_options()("help", "produce help message")(
        "random-permutation", po::value<std::vector<int>>()->multitoken(),
        "Use random permutation generator. Gets 4 numbers specifying number of CMYK containers, "
        "e.g. --random-permutation 4 4 4 4")("random-with-chance",
                                             po::value<std::vector<float>>()->multitoken(),
                                             "Use random with chance generator. Gets 2 numbers "
                                             "specifying the length of data and a chance "
                                             "that a containers will be at its correct positions")(
        "total-random", po::value<int>(),
        "Use total random generator. Argument is the length of data.")(
        "correct", po::value<int>(), "Use correct generator. Argument is the lenght of data.")(
        "naive", po::value<int>(),
        "use naive algorithm with specified optimization level. Possible values 0, 1, 2.")(
        "bfs", "use BFS algorithm")("partial", po::value<std::vector<int>>()->multitoken(),
                                    "use BFS in partial mode. Gets two ints as arguments. First "
                                    "specifies whether CMYKs have to be "
                                    "found on the front. Second specifies number of CMYKs to be "
                                    "found, e.g. --partial 0 2, returns "
                                    "subpath when two CMYKs are found anywhere in the sequence")(
        "astar", po::value<int>(),
        "use A* algorithm with specified heuristic function. Possible values 0, 1, 2.")
        ("iter", po::value<int>(), "specifies number of iterations")
        ("timeout", po::value<uint32_t>(), "specifies timeout in seconds");

    po::variables_map vm;
    try {
        po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
    } catch (std::exception& e) {
        cout << desc << "\n";
        return 1;
    }
    po::notify(vm);

    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }

    std::function<PropertyParser(const std::string&)> algorithm;
    std::string data;
    int iterations = 1;
    std::size_t timeout = 1;

    if (vm.count("iter")) {
        iterations = vm["iter"].as<int>();
        iterations = iterations > 0 ? iterations : 0;
    }
    if (vm.count("timeout")) {
        timeout = vm["timeout"].as<uint32_t>();
    }

    if (vm.count("naive")) {
        int optimization_level = vm["naive"].as<int>();
        if (optimization_level < 0 || optimization_level > 2) {
            std::cout << "naive: wrong optimization level" << std::endl;
            return 1;
        }
        std::cout << "Running naive with optimization level: " << optimization_level
                  << std::endl;
        algorithm = [optimization_level](const std::string& data) {
            return naive(data, optimization_level);
        };
    } else if (vm.count("bfs")) {
        std::cout << "Running BFS" << std::endl;
        algorithm = [timeout](const std::string& data) {
            return full_bfs(data, timeout);
        };
    } else if (vm.count("partial")) {
        std::vector<int> args = vm["partial"].as<std::vector<int>>();
        if (args.size() != 2) {
            std::cout << "partial: wrong number of arguments" << std::endl;
        }
        bool front = (bool)args[0];
        int seq_len = args[1];
        seq_len = seq_len > 0 ? seq_len : 0;
        printf("Running partial front=%d seq_len=%d\n", front, seq_len);
        algorithm = [front, seq_len, timeout](const std::string& data) {
            return partial_bfs(data, front, seq_len, timeout);
        };
    } else if (vm.count("astar")) {
        int heuristic = vm["astar"].as<int>();
        if (heuristic < 0 || heuristic > 2) {
            std::cout << "astar: wrong heuristic function number" << std::endl;
            return 1;
        }
        std::cout << "Running A* with heuristic number " << heuristic << std::endl;
        algorithm = [heuristic, timeout](const std::string& data) {
            return astar(data, heuristic, timeout);
        };
    } else {
        cout << desc << "\n";
        return 1;
    }

    if (vm.count("random-permutation")) {
        std::vector<int> counts = vm["random-permutation"].as<std::vector<int>>();
        if (counts.size() != 4) {
            std::cout << "random-permutation: wrong number of arguments" << std::endl;
            return 1;
        }
        printf("Data: random-permutation C=%d M=%d Y=%d K=%d\n", counts[0], counts[1], counts[2],
               counts[3]);
        data = Generator::random_permutation(
            RandomPermutationParams(counts[0], counts[1], counts[2], counts[3]));
    } else if (vm.count("random-with-chance")) {
        std::vector<float> args = vm["random-with-chance"].as<std::vector<float>>();
        if (args.size() != 2) {
            std::cout << "random-with-chance: wrong number of arguments" << std::endl;
            return 1;
        }
        int len = (int) args[0];
        float chance = std::clamp(args[1], 0.0f, 1.0f);
        printf("Data: random-with-chance len=%d chance=%f\n", len, chance);
        data = Generator::random_with_chance(len, chance);
    } else if (vm.count("total-random")) {
        int len = vm["total-random"].as<int>();
        len = len > 0 ? len : 0;
        std::cout << "Data: total-random len=" << len << std::endl;
        data = Generator::total_random(len);
    } else if (vm.count("correct")) {
        int len = vm["correct"].as<int>();
        len = len > 0 ? len : 0;
        std::cout << "Data: correct len=" << len << std::endl;
        data = Generator::correct(len);
    } else {
        std::cin >> data;
        std::cout << "Data: from file " << data << std::endl;
    }

    std::cout << "\ninitial: " << data << std::endl;
    benchmark(algorithm, data, iterations);

    return 0;
}
