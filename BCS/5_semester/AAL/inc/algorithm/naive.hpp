#ifndef __NAIVE_HPP__
#define __NAIVE_HPP__

#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <stdexcept>
#include <unordered_map>

#include "cmyk_algorithm.hpp"
#include "robot.hpp"

class Naive : public CMYKAlgorithm {
public:
    Naive(const Robot& robot, int optimization_level);
    AlgorithmResult solve() override;

private:
    // function called before main loop  of the algorithm it should pre sort the sequence
    std::function<void()> pre_solver;
    // executed after fitting chunk was moved. Checks for newly created CMYKs and moves them to the
    // front
    std::function<void(std::size_t)> cmyk_finder;
};

#endif
