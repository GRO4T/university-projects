#include "inc/algorithm/generator.hpp"

#include <algorithm>
#include <random>

std::string Generator::correct(std::size_t len) {
     std::string ret;
     for (std::size_t i=0; i < len; ++i)
         ret += "CMYK"[i % 4];
     return ret;
}

std::string Generator::total_random(std::size_t len) {
    std::string ret;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrb(0, 3);

    for (std::size_t i=0; i < len; ++i)
        ret += "CMYK"[distrb(gen)];

    return ret;
}

std::string Generator::random_permutation(const RandomPermutationParams& params) {
    std::string ret;

    std::fill_n(std::back_inserter(ret), params.C_n, 'C');
    std::fill_n(std::back_inserter(ret), params.M_n, 'M');
    std::fill_n(std::back_inserter(ret), params.Y_n, 'Y');
    std::fill_n(std::back_inserter(ret), params.K_n, 'K');

    std::random_shuffle(ret.begin(), ret.end());

    return ret;
}

char invalid(std::mt19937& gen, const std::size_t correct_pos) {
    std::uniform_int_distribution<> distrb(0, 2);
    std::string choice = "CMYK";
    choice.erase(correct_pos, 1);

    return choice.at(distrb(gen));
}

std::string Generator::random_with_chance(std::size_t len, double chance) {
    std::string ret;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distrb(0.0, 1.0);

    for (std::size_t i=0; i < len; ++i) {
        const char correct = "CMYK"[i % 4];

        if (distrb(gen) <= chance)
            ret += correct;
        else
            ret += invalid(gen, i % 4);
    }

    return ret;
}
