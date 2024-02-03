#ifndef __BFS_HPP__
#define __BFS_HPP__

#include <functional>
#include <iostream>
#include <list>

#include "cmyk_algorithm.hpp"

class FullBFS : public CMYKAlgorithm {
public:
    FullBFS(const Robot& robot) : CMYKAlgorithm(robot) {}
    AlgorithmResult solve() override;
};

class PartialBFS : public CMYKAlgorithm {
public:
    PartialBFS(const Robot& robot, bool front = false, int seq_len = 1);
    AlgorithmResult solve() override;

private:
    // when this function returns true we stop searching and return subpath
    std::function<bool(const NodePtr&)> termination_func;
    // this function generates subpath
    std::function<std::vector<std::size_t>(const NodePtr&)> path_generator;
};

#endif
