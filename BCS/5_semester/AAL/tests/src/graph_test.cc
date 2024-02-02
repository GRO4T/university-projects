#include <iostream>

#include "algorithm/generator.hpp"
#include "algorithm/graph.hpp"

int main() {
    CMYKGraph graph;
    auto init = Generator::correct(12);
    std::cout << init << "\n";
    auto node = graph.set_start(init);
    for (const auto & i : graph.get_neighbours(node)) {
        std::cout << i->state() << "\n";
    }
}
