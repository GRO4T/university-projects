#include "random_search.hpp"
#include "swarm_search.hpp"
#include <iostream>
#include "test_functions.hpp"
#include <cstring>

using namespace std;

void print_as_json(const SearchResult& search_result);
void print_help();

int main(int argc, char** argv)
{
    int n = 20;
    int threads = 4;
    int iterations = 100000;
    int particle_count = 100;
    int seconds = 5;
    double threshold = 0.9;
    int objective_func_id = 1;
    std::string search_algorithm = "random";
    std::string end_condition = "iteration";

    for (int i = 0; i < argc; ++i) {
        if ((strcmp(argv[i], "-n") == 0) || (strcmp(argv[i], "--dimension") == 0)) {
            n = stoi(argv[i+1]);
        } else if ((strcmp(argv[i], "-t") == 0) || (strcmp(argv[i], "--threads") == 0)) {
            threads = stoi(argv[i+1]);
        } else if ((strcmp(argv[i], "-i") == 0) || (strcmp(argv[i], "--iter") == 0)) {
            iterations = stoi(argv[i+1]);
            end_condition = "iteration";
        } else if ((strcmp(argv[i], "-p") == 0) || (strcmp(argv[i], "--particle") == 0)) {
            particle_count = stoi(argv[i+1]);
        } else if ((strcmp(argv[i], "-s") == 0) || (strcmp(argv[i], "--search") == 0)) {
            search_algorithm = argv[i+1];
        } else if ((strcmp(argv[i], "-f") == 0) || (strcmp(argv[i], "--obj_func") == 0)) {
            objective_func_id = stoi(argv[i+1]);
        } else if ((strcmp(argv[i], "-m") == 0) || (strcmp(argv[i], "--time") == 0)) {
            seconds = stoi(argv[i+1]);
            end_condition = "time";
        } else if ((strcmp(argv[i], "-l") == 0) || (strcmp(argv[i], "--threshold") == 0)) {
            threshold = stod(argv[i+1]);
            end_condition = "threshold";
        } else if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0)) {
            print_help();
            exit(0);
        }
    }

    std::cout << "---------------------------------------" << std::endl;
    std::cout << "n: " << n << std::endl;
    std::cout << "threads: " << threads << std::endl;
    if (end_condition == "iteration") {
        std::cout << "iterations: " << iterations << std::endl;
    } else if (end_condition == "time") {
        std::cout << "time limit: " << seconds << " [s]" << std::endl;
    } else if (end_condition == "threshold") {
        std::cout << "threshold to achieve: " << threshold << std::endl;
    }
    std::cout << "particle_count: " << particle_count << std::endl;
    std::cout << "search_algorithm: " << search_algorithm << std::endl;
    std::cout << "objective_func_id: " << objective_func_id << std::endl;
    std::cout << "---------------------------------------" << std::endl;

    SearchResult search_result;
    ObjectiveFunc objective_func = objective_func_id == 1 ? testFunc1 : testFunc2;
    double bound_x = objective_func_id == 1 ? 40 : 30;

    if (search_algorithm == "random") {
        RandomSearch random_search(objective_func, n, threads, -bound_x, bound_x);

        if (end_condition == "iteration") {
            search_result = random_search.search(iterations);
        } else if (end_condition == "time") {
            search_result = random_search.searchForSeconds(seconds);
        } else if (end_condition == "threshold") {
            search_result = random_search.searchUntilGreaterThan(threshold);
        }
    } else if (search_algorithm == "swarm") {
        SwarmSearch swarm_search(objective_func, n, particle_count, threads, -bound_x, bound_x);
        if (end_condition == "iteration") {
            search_result = swarm_search.search(iterations);
        } else if (end_condition == "time") {
            search_result = swarm_search.searchForSeconds(seconds);
        } else if (end_condition == "threshold") {
            search_result = swarm_search.searchUntilGreaterThan(threshold);
        }
    }

    std::cout << "Search: " << search_algorithm << std::endl;
    std::cout << "Result:\n";
    print_as_json(search_result);
}

void print_as_json(const SearchResult& search_result) {
    cout << "{\n";
    std::cout << "\t\"value\": " << search_result.result << ",\n";
    std::cout << "\t\"position\": [";
    for(auto x : search_result.x)
        cout << x << ", ";
    std::cout << "],\n";

    std::cout << "\t\"exec_time_in_nanos\": " << search_result.time << ",\n";
    std::cout << "\t\"exec_time\": " << (double) search_result.time / 1000000000.0 << ",\n";
    std::cout << "\t\"iterations\": " << search_result.iterations << "\n";

    std::cout << "}\n";
}

void print_help() {
    std::cout << "usage: runner [-h | --help] [-n | --dimension] DIMENSION [-t | --threads] THREADS\n"
              << "[-i | --iterations] ITERATIONS [-p | --particle] PARTICLE_COUNT [-s | --search] SEARCH_ALGORITHM [-f | --obj_func] OBJ_FUNC\n"
              << "[-m | --time] TIME [-l | --threshold] THRESHOLD\n"
              << "\tdimension - number of dimensions of test function\n"
              << "\tthreads - number of threads\n"
              << "\titer - finish after X iterations\n"
              << "\tsearch - search algorithm (random or swarm)\n"
              << "\tobj_func - id of test function (1 or 2)\n"
              << "\ttime - finish after X seconds\n"
              << "\tthreshold - finish after best result reaches X\n"
              ;
}