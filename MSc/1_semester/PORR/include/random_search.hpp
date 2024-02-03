#ifndef RANDOM_SEARCH
#define RANDOM_SEARCH

#include <ctime>
#include <random>
#include <functional>
#include <future>

#include "utilities.hpp"

using Unifs = std::vector<std::uniform_real_distribution<double>>;

class RandomSearch
{
public:
    RandomSearch(
        std::function<double(Point)> &objective_func,
        size_t n,
        int threads,
        double min_x,
        double max_x
    );

    void setSeed(int thread_id);

    SearchResult search(size_t iterations);
    SearchResult searchForSeconds(int seconds);
    SearchResult searchUntilGreaterThan(double threshold);

    void forceStop() { force_stop = true; }

    void plot(size_t iterations, double animation_speed = 1.0 / 0.01);

    static thread_local std::mt19937 random_engine;

private:
    std::function<double(Point)> objective_func;
    size_t n;
    int threads;
    double min_x;
    double max_x;
    uint time_seed;

    bool force_stop = false;

    Unifs getUnifs();
    SearchResult searchUntilStopped();
};

#endif