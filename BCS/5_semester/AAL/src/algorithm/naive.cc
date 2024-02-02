#include "inc/algorithm/naive.hpp"

#include <stdexcept>
#include <unordered_set>

#include "inc/algorithm/property.hpp"

Naive::Naive(const Robot &robot, int optimization_level) : CMYKAlgorithm(robot) {
    switch (optimization_level) {
        case 0:
            pre_solver = []() {};
            cmyk_finder = [this](std::size_t len) {
                // if CMYK found at the back move it to the front
                if (this->robot.state().find(CMYK, len - 4) != std::string::npos) {
                    cmyk_front();
                }
            };
            break;
        case 1:
            pre_solver = []() {};
            cmyk_finder = [this](std::size_t len) {
                // while there are CMYKs somewhere move them to the front
                while (this->robot.state().find(CMYK, already_sorted) != std::string::npos &&
                       (len - already_sorted) > 9) {
                    auto pos = this->robot.state().find(CMYK, already_sorted);
                    execute_move(pos);
                    cmyk_front();
                }
            };
            break;
        case 2:
            pre_solver = [this]() {
                // search for CMYKs and move them to the front
                std::size_t len = this->robot.state().length();
                while (this->robot.state().find(CMYK, already_sorted) != std::string::npos &&
                       (len - already_sorted) > 9) {
                    auto pos = this->robot.state().find(CMYK, already_sorted);
                    execute_move(pos);
                    cmyk_front();
                }
            };
            cmyk_finder = [this](std::size_t len) {
                // while there are CMYKs somewhere move them to the front
                while (this->robot.state().find(CMYK, already_sorted) != std::string::npos &&
                       (len - already_sorted) > 9) {
                    auto pos = this->robot.state().find(CMYK, already_sorted);
                    execute_move(pos);
                    cmyk_front();
                }
            };
            break;
    }
}

AlgorithmResult Naive::solve() {
    if (cannot_sort_further_strict(robot.state()))
        return PropertyBuilder().set_success(true).set_sorted_string(robot.state());

    pre_solver();
    std::unordered_set<std::string> visited;

    // (len - already_sorted) > 9 ensures that after leaving this loop sequence will
    // be sortable using full BFS
    const std::size_t len = this->robot.state().length();
    while (!cannot_sort_further_naive(this->robot.state()) && (len - already_sorted) > 9) {
        const char last = this->robot.state()[len - 1];
        const auto already_build_CMYK_fragment_len = CMYK.find(last);
        const char next_in_seq = CMYK[(already_build_CMYK_fragment_len + 1) % 4];
        const size_t fitting_chunk_pos = this->robot.state().find(next_in_seq, already_sorted);

        // this condition checks if by moving the 'fitting_chunk' we would move 'already_build_fragment' (this leads to loop)
        if (fitting_chunk_pos > len - already_build_CMYK_fragment_len - 4) {
            execute_move(fitting_chunk_pos - 3);
            continue;
        }
        if (fitting_chunk_pos < len - 3) {
            execute_move(fitting_chunk_pos);
            execute_move(len - 7);
            cmyk_finder(len);
        }

        if (visited.contains(robot.state())) {
            std::cerr << "Naive has looped\n";
            break;
        }
        visited.insert(robot.state());
        /* std::cout << robot.state() << "\n"; */
        /* printf("cmp: %d, %d\n", cannot_sort_further_naive(this->robot.state()), len - already_sorted); */
    }

    // sequence is this long naive must have broken the loop (sequence this long isn't practical for BFS)
    if (len - already_sorted > 12 && !cannot_sort_further_strict(robot.state())) {
        return PropertyBuilder().set_success(false).set_fail_reson("Unsortable string");
    }

    // sort last six to ten containers using BFS (six is the least amount sortable)
    if (!cannot_sort_further_strict(robot.state())) {
        auto node = graph.set_start(robot.state().substr(already_sorted));
        auto path = bfs(
            std::move(node),
            [this](const NodePtr &n) { return cannot_sort_further_strict(n->state()); },
            [this](const NodePtr &n) { return graph.reconstruct_path(n); });
        execute_path(path, already_sorted);
    }

    return PropertyBuilder()
        .set_moves(total_path)
        .set_success(cannot_sort_further_strict(robot.state()))
        .set_sorted_string(robot.state());
}
