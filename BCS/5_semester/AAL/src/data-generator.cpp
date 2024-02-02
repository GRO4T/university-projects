#include <boost/program_options.hpp>
#include <functional>
#include <iomanip>
#include <iostream>

#include "algorithm/generator.hpp"
#include "inc/algorithm/algorithm.hpp"

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    using namespace std;
    po::options_description desc("Usage: data-generator <generator> [options]");
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
        "correct", po::value<int>(), "Use correct generator. Argument is the lenght of data.");

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

    std::string data;



    if (vm.count("random-permutation")) {
        std::vector<int> counts = vm["random-permutation"].as<std::vector<int>>();
        if (counts.size() != 4) {
            std::cout << "random-permutation: wrong number of arguments" << std::endl;
            return 1;
        }
        data = Generator::random_permutation(
            RandomPermutationParams(counts[0], counts[1], counts[2], counts[3]));
    } else if (vm.count("random-with-chance")) {
        std::vector<float> args = vm["random-with-chance"].as<std::vector<float>>();
        if (args.size() != 2) {
            std::cout << "random-with-chance: Wrong number of arguments" << std::endl;
            return 1;
        }
        int len = (int) args[0];
        float chance = std::clamp(args[1], 0.0f, 1.0f);
        data = Generator::random_with_chance(len, chance);
    } else if (vm.count("total-random")) {
        int len = vm["total-random"].as<int>();
        len = len > 0 ? len : 0;
        data = Generator::total_random(len);
    } else if (vm.count("correct")) {
        int len = vm["correct"].as<int>();
        len = len > 0 ? len : 0;
        data = Generator::correct(len);
    } else {
        std::cout << desc << std::endl;
        return 1;
    }

    std::cout << data << std::endl;

    return 0;
}
