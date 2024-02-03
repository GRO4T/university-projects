#include "inc/algorithm/astar.hpp"

#include "inc/algorithm/property.hpp"

Fullastar::Fullastar(const Robot& robot, std::size_t heurustic_function_index)
    : CMYKAlgorithm(robot) {
    expected_number_of_cmyks = CMYKAlgorithm::count_expected_cmyks();

    switch (heurustic_function_index) {
        case 0:
            heurustic_function = [this](const NodePtr& node) {
                return heura_correct_subsequences_len(node);
            };
            break;
        case 1:
            heurustic_function = [this](const NodePtr& node) {
                return heura_correct_position(node);
            };
            break;
        case 2:
            heurustic_function = [this](const NodePtr& node) {
                return heura_number_of_cmyks(node);
            };
            break;
    }
}

AlgorithmResult Fullastar::solve() {
    if (cannot_sort_further_strict(robot.state()))
        return PropertyBuilder().set_success(true).set_sorted_string(robot.state());

    auto node = graph.set_start(robot.state());
    std::function<std::size_t(const NodePtr&)> heura;

    auto path = astar(
        std::move(node),
        [this](const NodePtr& n) { return cannot_sort_further_strict(n->state()); },
        [this](const NodePtr& n) { return graph.reconstruct_path(n); }, heurustic_function);
    execute_path(path);

    return PropertyBuilder()
        .set_moves(path)
        .set_success(cannot_sort_further_strict(robot.state()))
        .set_sorted_string(robot.state());
}

std::size_t Fullastar::heura_number_of_cmyks(const NodePtr& node) const {
    std::size_t count = 0;
    std::size_t pos = 0;
    const std::string& state = node->state();
    while ((pos = state.find("CMYK", pos + 4)) != std::string::npos) count++;

    return expected_number_of_cmyks - count;
}

std::size_t Fullastar::heura_correct_position(const NodePtr& node) const {
    std::size_t count = 0;
    const std::string& state = node->state();
    for (std::size_t i = 0; i < expected_number_of_cmyks * 4; ++i)
        if (state[i] == "CMYK"[i % 4]) count++;

    return expected_number_of_cmyks - count;
}

std::size_t Fullastar::heura_correct_subsequences_len(const NodePtr& node) const {
    std::size_t len = 0;
    const std::string& state = node->state();
    for (std::size_t i = 0; i < (expected_number_of_cmyks * 4 - 1); ++i)
        if (CMYK.find(state[i]) == ((CMYK.find(state[i + 1]) + 1) % 4)) len++;

    return expected_number_of_cmyks * 4 - 1 - len;
}
