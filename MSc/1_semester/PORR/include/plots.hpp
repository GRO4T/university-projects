#ifndef PLOTS_HPP
#define PLOTS_HPP

#include <functional>
#include <future>
#include <vector>

#include "utilities.hpp"

using TestFunction = std::function<double(std::vector<double>)>;

void display3DSurfacePlot(const TestFunction& func);
void plotClear();
void plotContourWithBestAndCurrentPoint(
    const TestFunction& objective_func,
    const Point& best_point,
    const Point& current_point,
    double min_x,
    double max_x,
    double animation_speed
);
void plotContourWithQuiver(
    const TestFunction& objective_func,
    const Point& best,
    const std::vector<double>& x,
    const std::vector<double>& y,
    const std::vector<double>& u,
    const std::vector<double>& v,
    double min_x,
    double max_x,
    double animation_speed
);

#endif