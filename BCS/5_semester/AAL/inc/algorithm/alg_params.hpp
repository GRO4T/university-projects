#ifndef __ALG_PARAMS_HPP__
#define __ALG_PARAMS_HPP__

#include <string>
#include <unordered_map>
#include <vector>

struct AlgorithmResult {
    std::vector<std::size_t> moves;
    std::unordered_map<std::string, std::string> results;
};

struct AdditionalNaiveParams {
    std::size_t opti_level;
};

struct AdditionalAstarParams {
    std::size_t heurustic_function;
};

struct AdditionalBFSParams {
    enum ReturnOn { Sorted, CMYKFound } return_on;

    bool is_front;
    std::size_t seq_len;
};

struct AlgorithmParams {
    std::string containers;
    double timeout;
    bool timeout_enabled;
};

struct RandomPermutationParams {
    RandomPermutationParams(size_t cN, size_t mN, size_t yN, size_t kN)
        : C_n(cN), M_n(mN), Y_n(yN), K_n(kN) {}
    std::size_t C_n;
    std::size_t M_n;
    std::size_t Y_n;
    std::size_t K_n;
};

#endif
