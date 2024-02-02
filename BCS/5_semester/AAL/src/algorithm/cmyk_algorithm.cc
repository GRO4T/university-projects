#include <inc/algorithm/cmyk_algorithm.hpp>
#include <queue>
#include <unordered_set>

std::vector<std::size_t> CMYKAlgorithm::bfs(
    NodePtr node, std::function<bool(NodePtr&)> termination_func,
    std::function<std::vector<std::size_t>(NodePtr&)> path_generator) {
    return basic_bfs(std::move(node), termination_func, path_generator, std::queue<NodePtr>());
}

std::vector<std::size_t> CMYKAlgorithm::astar(
    NodePtr node, std::function<bool(NodePtr&)> termination_func,
    std::function<std::vector<std::size_t>(NodePtr&)> path_generator,
    std::function<std::size_t(const NodePtr&)> hfunc) {
    auto cmp = [hfunc](const NodePtr& a, const NodePtr& b) { return hfunc(a) > hfunc(b); };

    return basic_bfs(std::move(node), termination_func, path_generator,
                     std::priority_queue<NodePtr, std::deque<NodePtr>, decltype(cmp)>(cmp));
}

// check if sequence is of the following type CMYKX, MYKXC, YKXCM, ...
bool cmyk_begins_at(const std::string& state, std::size_t at) {
    for (std::size_t i = 0; i < 4; ++i) {
        if (state[(at + i) % 5] != "CMYK"[i]) return false;
    }
    return true;
}

bool CMYKAlgorithm::cannot_sort_further_naive(const std::string& state) {
    std::size_t sorted = 0;
    std::size_t pos;

    while ((pos = state.substr(sorted).find(CMYK)) != std::string::npos && pos == 0) sorted += 4;

    if (sorted == state.length())  // fully sorted
        return true;

    std::string unsorted = state.substr(sorted);
    std::array<std::size_t, 4> character_count{};
    for (std::size_t i = 0; i < character_count.size(); ++i)
        character_count[i] = std::count(unsorted.begin(), unsorted.end(), "CMYK"[i]);

    auto min_count = *std::min_element(character_count.begin(), character_count.end());
    if (min_count == 0) return true; // this means that unsorted part lacks one or more of the colors

    if (unsorted.size() == 5) {
        for (std::size_t i = 0; i < 5; ++i) {
            if (cmyk_begins_at(unsorted, i)) return false;
        }
        return true;
    } else if (unsorted.size() <= 4) {
        return true;
    }
    return false;
}

bool CMYKAlgorithm::cannot_sort_further_strict(const std::string& state) {
    std::size_t i;
    for (i = 0; i < state.length(); ++i) {
        if (state[i] != CMYK[i % 4]) break;
    }

    std::string unsorted = state.substr(i);
    for (std::size_t j = i % 4; j < 4; ++j)
        if (unsorted.find(CMYK[j]) == std::string::npos) return true;

    return false;
}

void CMYKAlgorithm::cmyk_front() {
    while (robot.state().find(CMYK, already_sorted) != already_sorted) execute_move(already_sorted);

    already_sorted += 4;
}

const std::string& CMYKAlgorithm::execute_path(const std::vector<std::size_t>& path,
                                               std::size_t offset) {
    for (const auto& move : path) execute_move(move, offset);

    return robot.state();
}

const std::string& CMYKAlgorithm::execute_move(std::size_t move, std::size_t offset) {
    robot.move(move + offset);
    total_path.push_back(move + offset);

    return robot.state();
}

std::size_t CMYKAlgorithm::count_expected_cmyks() const {
    std::array<std::size_t, 4> counts;
    for (std::size_t i = 0; i < 4; ++i)
        counts[i] = std::count(robot.state().begin(), robot.state().end(), "CMYK"[i]);
    return *std::min_element(counts.begin(), counts.end());
}
