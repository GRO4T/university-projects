#include "algorithm/algorithm.hpp"
#include "algorithm/bfs.hpp"
#include "algorithm/generator.hpp"

int main() {
    /* std::cout << "Partial BFS final: " << partial_bfs(Generator::total_random(8) + "M") << std::endl; */
    /* //std::cout << "Partial BFS final: " << full_bfs("CMMYK") << std::endl; */
    /* std::cout << "Partial BFS final: " << partial_bfs("CMMYK") << std::endl; */

    std::cout << "Full Bfs: " << full_bfs("KYMCKYMCKYMC").get_sorted_string() << "\n";

    return 0;
}

