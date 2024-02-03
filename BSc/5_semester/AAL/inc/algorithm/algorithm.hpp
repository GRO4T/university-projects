#ifndef CMYK_ALGORITHM_H
#define CMYK_ALGORITHM_H

#include "algorithm/astar.hpp"
#include "algorithm/bfs.hpp"
#include "naive.hpp"
#include "property.hpp"

// header containing convenient wrappers for our algorithms

inline PropertyParser full_bfs(const std::string& initial_state, std::size_t timeout = 60) {
    Robot robot(initial_state);
    FullBFS full_bfs(robot);
    full_bfs.setTimeout(timeout);
    try {
        auto result = full_bfs.solve();
        return result;
    } catch (std::runtime_error& e) {
        return PropertyBuilder().set_success(false).set_fail_reson(e.what()).build();
    }
}

inline auto full_bfs_unit_tests_wrapper() {
    return [](const std::string& data) { return full_bfs(data); };
}

inline PropertyParser partial_bfs(const std::string& initial_state, bool front, int seq_len,
                                  std::size_t timeout = 60) {
    Robot robot(initial_state);
    PartialBFS partial_bfs(robot, front, seq_len);
    partial_bfs.setTimeout(timeout);
    try {
        auto result = partial_bfs.solve();
        return result;
    } catch (std::runtime_error& e) {
        return PropertyBuilder().set_success(false).set_fail_reson(e.what()).build();
    }
}

inline auto partial_bfs_unit_tests_wrapper(bool front, int seq_len) {
    return [front, seq_len](const std::string& data) { return partial_bfs(data, front, seq_len); };
}

inline PropertyParser naive(const std::string& initial_state, int optimization_level) {
    Robot robot(initial_state);
    Naive naive(robot, optimization_level);
    return PropertyParser(naive.solve());
}

inline auto naive_unit_tests_wrapper(int optimization) {
    return [optimization](const std::string& data) { return naive(data, optimization); };
}

inline PropertyParser astar(const std::string& initial_state, std::size_t heura = 0,
                            std::size_t timeout = 60) {
    Robot robot(initial_state);
    Fullastar astar(robot, heura);
    astar.setTimeout(timeout);
    try {
        auto result = astar.solve();
        return result;
    } catch (std::runtime_error& e) {
        return PropertyBuilder().set_success(false).set_fail_reson(e.what()).build();
    }
}

inline auto astar_unit_tests_wrapper(std::size_t heura) {
    return [heura](const std::string& data) { return astar(data, heura); };
}

#endif  // CMYK_ALGORITHM_H
