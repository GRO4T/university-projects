#include <cmath>
#include <thread>
#include <cstring>
#include <iostream>

#include "test_functions.hpp"
#include "random_search.hpp"
#include "swarm_search.hpp"
#include "plots.hpp"

void print_help();

int main(int argc, char** argv)
{
    int n = 2;
    int objective_func_id = 1;
    std::string search_algorithm = "random";
    int iterations = 1000;

    for (int i = 0; i < argc; ++i) {
        if ((strcmp(argv[i], "-n") == 0) || (strcmp(argv[i], "--dimension") == 0)) {
            n = std::stoi(argv[i+1]);
        } else if ((strcmp(argv[i], "-i") == 0) || (strcmp(argv[i], "--iterations") == 0)) {
            iterations = std::stoi(argv[i+1]);
        } else if ((strcmp(argv[i], "-s") == 0) || (strcmp(argv[i], "--search") == 0)) {
            search_algorithm = argv[i+1];
        } else if ((strcmp(argv[i], "-f") == 0) || (strcmp(argv[i], "--obj_func") == 0)) {
            objective_func_id = std::stoi(argv[i+1]);
        } else if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0)) {
            print_help();
            exit(0);
        }
    }

    std::cout << "---------------------------------------" << std::endl;
    std::cout << "n: " << n << std::endl;
    std::cout << "iterations: " << iterations << std::endl;
    std::cout << "search_algorithm: " << search_algorithm << std::endl;
    std::cout << "objective_func_id: " << objective_func_id << std::endl;
    std::cout << "---------------------------------------" << std::endl;

    ObjectiveFunc objective_func = objective_func_id == 1 ? testFunc1 : testFunc2;

    auto random_search_2d = [](int iterations, ObjectiveFunc obj_func) {
        RandomSearch random_search(obj_func, 2, 1, -40, 40);
        random_search.plot(1000);
    };

    auto swarm_search_2d = [](int iterations, ObjectiveFunc obj_func) {
        SwarmSearch swarm_search(obj_func, 2, 50, 1, -40, 40);
        swarm_search.plot(1000);
    };

    auto surface_plot_3d = [](const ObjectiveFunc& objective_func) {
        display3DSurfacePlot(objective_func);
        std::terminate(); // WORKAROUND: matplotlib won't close the window
    };

    if (n == 3) {
        std::thread surface_plotter(surface_plot_3d, objective_func);
        surface_plotter.join();
    } else if (n == 2) {
        if (search_algorithm == "random") {
            std::thread animated_contour_plotter(random_search_2d, iterations, objective_func);
            animated_contour_plotter.join();
        } else if (search_algorithm == "swarm") {
            std::thread animated_contour_plotter(swarm_search_2d, iterations, objective_func);
            animated_contour_plotter.join();
        }
    }

    return 0;
}

void print_help() {
    std::cout << "usage: plotter [-h | --help] [-n | --dimension] DIMENSION\n"
              << "[-i | --iterations] ITERATIONS [-s | --search] SEARCH_ALGORITHM [-f | --obj_func] OBJ_FUNC\n";
}