#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <vector>
#include <chrono>
#include <functional>

#define  OPENMP_ENABLED // wykomentować jeśli nie chcemy zrównoleglenia

using Point = std::vector<double>;
using ObjectiveFunc = std::function<double(Point)>;

struct SearchResult
{
    Point x;
    double result;
    long int time;
    size_t iterations;
};


#endif