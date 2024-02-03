#include "inc/algorithm/bfs.hpp"

#include "inc/algorithm/property.hpp"

AlgorithmResult FullBFS::solve() {
    if (cannot_sort_further_strict(robot.state()))
        return PropertyBuilder().set_success(true).set_sorted_string(robot.state());

    auto node = graph.set_start(robot.state());
    auto path = bfs(
        std::move(node),
        [this](const NodePtr &n) { return cannot_sort_further_strict(n->state()); },
        [this](const NodePtr &n) { return graph.reconstruct_path(n); });
    execute_path(path);

    return PropertyBuilder()
        .set_moves(total_path)
        .set_success(cannot_sort_further_strict(robot.state()))
        .set_sorted_string(robot.state());
}

PartialBFS::PartialBFS(const Robot &robot, bool front, int seq_len) : CMYKAlgorithm(robot) {
    std::string seq = "";
    for (int i = 0; i < seq_len; i++) seq += CMYK;
    if (front) {
        termination_func = [this, seq, seq_len](const NodePtr &n) {
            // if sequence is long enough search for seq_len number of CMYKs at the front
            if (n->state().length() > 4 * seq_len + 5) {
                auto pos = n->state().find(seq);
                return pos != std::string::npos && pos == 0;
            } else { // search for one CMYK at the front
                auto pos = n->state().find(CMYK);
                return pos != std::string::npos && pos == 0;
            }
        };
        path_generator = [this](const NodePtr &n) { return graph.reconstruct_path(n); };
    } else {
        termination_func = [this, seq, seq_len](const NodePtr &n) {
            // if sequence is long enough search for seq_len number CMYKs
            if (n->state().length() > 4 * seq_len + 5) {
                return n->state().find(seq) != std::string::npos;
            } else { // search for one CMYK
                return n->state().find(CMYK) != std::string::npos;
            }
        };
        path_generator = [this, seq_len](const NodePtr &n) {
            // if sequence was long enough: reconstruct path then add moves putting
            // found CMYKs at the end of the sequence
            if (n->state().length() > 4 * seq_len + 5) {
                auto path = graph.reconstruct_path(n);
                for (int i = 0; i < seq_len; i++) {
                    auto pos = n->state().find(CMYK);
                    path.push_back(pos);
                }
                return path;
            } else { // reconstruct path and add a move putting found CMYK at the end
                auto path = graph.reconstruct_path(n);
                auto pos = n->state().find(CMYK);
                path.push_back(pos);
                return path;
            }
        };
    }
}

AlgorithmResult PartialBFS::solve() {
    if (cannot_sort_further_strict(robot.state()))
        return PropertyBuilder().set_success(true).set_sorted_string(robot.state());

    const auto len = robot.state().length();
    auto node = graph.set_start(robot.state());
    // (len - already_sorted) > 9 ensures that after leaving this loop sequence will
    // be sortable using full BFS
    while (!cannot_sort_further_naive(robot.state()) && (len - already_sorted) > 9) {
        auto path = bfs(std::move(node), termination_func, path_generator);

        execute_path(path, already_sorted);
        cmyk_front();

        node = graph.set_start(robot.state().substr(already_sorted));
    }
    // full BFS
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
