#ifndef __ASTAR_HPP__
#define __ASTAR_HPP__

#include "cmyk_algorithm.hpp"

class Fullastar : public CMYKAlgorithm {
    std::size_t expected_number_of_cmyks;
    std::function<std::size_t(const NodePtr&)> heurustic_function;

    std::size_t heura_number_of_cmyks(const NodePtr& node) const;
    std::size_t heura_correct_position(const NodePtr& node) const;
    std::size_t heura_correct_subsequences_len(const NodePtr& node) const;

public:
    Fullastar(const Robot& robot, std::size_t heurustic_function);
    AlgorithmResult solve() override;
};

#endif
