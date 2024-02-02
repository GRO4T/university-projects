#include "inc/gui/aalbackend.h"

#include <algorithm>
#include <inc/algorithm/naive.hpp>
#include <thread>

#include "inc/algorithm/astar.hpp"
#include "inc/algorithm/bfs.hpp"
#include "inc/algorithm/generator.hpp"

AALBackend::AALBackend() {

}

void AALBackend::computeBFS(AlgorithmParams params, AdditionalBFSParams additional_params) {
    if (additional_params.return_on == AdditionalBFSParams::Sorted) {
        std::thread([this, params = std::move(params), additional_params = std::move(additional_params)] () {
          FullBFS bfs(params.containers);
          computationFinished(bfs.solve());
        }).detach();
    }
    else {
        std::thread([this, params = std::move(params), additional_params = std::move(additional_params)] () {
          PartialBFS partial_bfs(params.containers, additional_params.is_front, additional_params.seq_len);
          computationFinished(partial_bfs.solve());
        }).detach();
    }
}

void AALBackend::computeNaive(AlgorithmParams params, AdditionalNaiveParams additional_params) {
    std::thread([this, params = std::move(params), additional_params = std::move(additional_params)] () {
      Naive naive(params.containers, additional_params.opti_level);
      computationFinished(naive.solve());
    }).detach();
}

void AALBackend::computeAstar(AlgorithmParams params, AdditionalAstarParams additional_params) {
    std::thread([this, params = std::move(params), additional_params = std::move(additional_params)] () {
        Fullastar astar(params.containers, additional_params.heurustic_function);
        computationFinished(astar.solve());
    }).detach();
}

std::string AALBackend::generateRandom(std::size_t len) {
    return Generator::total_random(len);
}

std::string AALBackend::generateRandomPermutation(RandomPermutationParams params) {
    return Generator::random_permutation(params);
}

std::string AALBackend::generateRandomWithChance(std::size_t len, double chance) {
    return Generator::random_with_chance(len, chance);
}
