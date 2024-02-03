#ifndef SWARM_SEARCH
#define SWARM_SEARCH

#include <ctime>
#include <random>
#include <vector>
#include <cfloat>
#include <functional>

#include "utilities.hpp"

struct Particle
{
    std::vector<double> position;
    std::vector<double> velocity;
    SearchResult bestLocalResult;
};

class SwarmSearch
{
public:
    SwarmSearch(
        std::function<double(Point)> &objective_func,
        size_t n,
        size_t particle_count,
        int threads,
        double min_x,
        double max_x
    );

    void setSeed(int thread_id);

    SearchResult search(size_t iterations);
    SearchResult searchForSeconds(int seconds);
    SearchResult searchUntilGreaterThan(double threshold);

    void plot(size_t iterations, double animation_speed = 1.0 / 0.01);
    void forceStop() { force_stop = true; }

    static thread_local std::mt19937 random_engine;
private:
    std::function<double(Point)> objective_func;
    size_t n;
    size_t particle_count;
    int threads;
    double min_x;
    double max_x;
    uint time_seed;
    bool force_stop = false;

    double c1 = 1;
    const double c2 = 0.8, c3 = 0.8;
    SearchResult best_global_result;
    std::vector<Particle> particles;
    std::uniform_real_distribution<double> unif01 = std::uniform_real_distribution<double>(0, 1);

    void init();
    void updateParticle(Particle& particle);
    void updateVelocity(Particle& particle);
    SearchResult searchUntilStopped();
};


#endif