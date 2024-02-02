#ifndef __GENERATOR_HPP__
#define __GENERATOR_HPP__

#include <string>

#include "alg_params.hpp"

class Generator {
public:
    static std::string random_permutation(const RandomPermutationParams& params);
    static std::string random_with_chance(std::size_t len, double chance);
    static std::string total_random(std::size_t len);
    static std::string correct(std::size_t len);
};

#endif
