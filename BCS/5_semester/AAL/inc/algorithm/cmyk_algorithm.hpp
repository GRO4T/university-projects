#ifndef CMYK_COMMON_HPP
#define CMYK_COMMON_HPP

#include <bits/c++config.h>

#include <array>
#include <chrono>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <type_traits>
#include <unordered_set>

#include "alg_params.hpp"
#include "graph.hpp"
#include "robot.hpp"

inline std::ostream& operator<<(std::ostream& os, const Robot& robot) {
    std::cout << robot.get_moves_done() << ": " << robot.state() << std::endl;
    return os;
}

using NodePtr = std::unique_ptr<CMYKGraph::Node>;
static const std::string CMYK = "CMYK";

/* Base class for our algorithms */
class CMYKAlgorithm {
public:
    CMYKAlgorithm(const Robot& robot) : robot(robot) {}
    void setTimeout(std::size_t value) { timeout = value; }
    virtual AlgorithmResult solve() = 0;
    const Robot& getRobot() const { return robot; }

private:
protected:
    std::size_t timeout = 60;

    Robot robot;
    std::size_t already_sorted = 0;
    CMYKGraph graph;
    std::vector<std::size_t> total_path;

    /* generic BFS
     * Arguments:   termination_func - when returns true, stop searching and return subpath
     *              path_generator - creates subpath
     * Returns: vector of moves leading to where we stopped */
    std::vector<std::size_t> bfs(NodePtr node, std::function<bool(NodePtr&)> termination_func,
                                 std::function<std::vector<std::size_t>(NodePtr&)> path_generator);
    /* generic A*
     * Arguments:   termination_func - when returns true, stop searching and return subpath
     *              path_generator - creates subpath
     *              hfunc - heuristic function
     * Returns: vector of moves leading to where we stopped */
    std::vector<std::size_t> astar(NodePtr node, std::function<bool(NodePtr&)> termination_func,
                                   std::function<std::vector<std::size_t>(NodePtr&)> path_generator,
                                   std::function<std::size_t(const NodePtr&)> hfunc);

    // returns true if sequence cannot be sorted without destroying previously sorted elements
    bool cannot_sort_further_naive(const std::string& state);
    // returns true if sequence is not sortable at all
    bool cannot_sort_further_strict(const std::string& state);

    void cmyk_front();
    const std::string& execute_path(const std::vector<std::size_t>& path, std::size_t offset = 0);
    const std::string& execute_move(std::size_t move, std::size_t offset = 0);

    /* template used by A* */
    template <typename Queue>
    std::vector<std::size_t> basic_bfs(
        NodePtr node, std::function<bool(NodePtr&)> termination_func,
        std::function<std::vector<std::size_t>(NodePtr&)> path_generator, Queue queue);

    // heuristic function estimation of the number of CMYKs in sorted sequence
    std::size_t count_expected_cmyks() const;
};

// templated functions

template <typename Queue>
struct has_top : std::true_type {};
template <typename T>
struct has_top<std::queue<T>> : std::false_type {};

template <typename Queue, std::enable_if<has_top<Queue>::value, int>::type = 0>
NodePtr queue_top(Queue& queue) {
    return std::move(const_cast<NodePtr&>(queue.top()));
}
template <typename Queue, std::enable_if<!has_top<Queue>::value, int>::type = 0>
NodePtr queue_top(Queue& queue) {
    return std::move(queue.front());
}

template <typename Queue>
std::vector<std::size_t> CMYKAlgorithm::basic_bfs(
    NodePtr node, std::function<bool(NodePtr&)> termination_func,
    std::function<std::vector<std::size_t>(NodePtr&)> path_generator, Queue nodes_to_search) {
    std::unordered_set<std::string> visited_states;

    visited_states.insert(node->state());
    nodes_to_search.push(std::move(node));

    auto t1 = std::chrono::steady_clock::now();
    std::size_t execution_time;
    while (!nodes_to_search.empty()) {
        auto t2 = std::chrono::steady_clock::now();
        execution_time = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
        if (execution_time > timeout) {
            throw std::runtime_error("timeout!");
        }

        NodePtr current_node = queue_top(nodes_to_search);
        nodes_to_search.pop();
        auto neighbours = graph.get_neighbours(current_node);
        while (!neighbours.empty()) {
            auto neighbour = std::move(neighbours.back());
            neighbours.pop_back();
            graph.set_predecessor(current_node, neighbour);
            if (termination_func(neighbour)) {
                return path_generator(neighbour);
            }
            // update visited states
            if (!visited_states.contains(neighbour->state())) {
                visited_states.insert(neighbour->state());
                nodes_to_search.push(std::move(neighbour));
            }
        }
    }
    return {};
}

#endif  // CMYK_COMMON_HPP
