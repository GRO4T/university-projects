#include "algorithm/astar.hpp"
#include "algorithm/algorithm.hpp"
#include "algorithm/generator.hpp"

int main() {
    std::cout << "full astar final: " << astar(Generator::total_random(8) + "M").get_sorted_string() << std::endl;
    //std::cout << "full astar final: " << full_astar("CMMYK") << std::endl;
    std::cout << "full astar final: " << astar("KMYCKMYC",2).get_sorted_string() << std::endl;


    return 0;
}

