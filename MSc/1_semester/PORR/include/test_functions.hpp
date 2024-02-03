#ifndef TEST_FUNCTIONS_HPP
#define TEST_FUNCTIONS_HPP

#include <vector>
#include <numeric>
#include <cmath>

#include <iostream>

#include "utilities.hpp"


template<typename T>
struct square
{
    T operator()(const T& Left, const T& Right) const
    {   
        return (Left + Right*Right);
    }
};

double calculateTestFunc1CosinusPart(const Point& point);

double calculateTestFunc2CosinusPart(const Point& point);


double testFunc1(Point point);


double testFunc2(Point point);

#endif