#include "inc/algorithm/robot.hpp"
#include <stdexcept>

const std::string& Robot::move(std::size_t m) {
    if (m >= array.length() - 3) 
        throw std::runtime_error("Error invalid move");

    array.append(array.substr(m, 4));
    array.erase(m, 4);
    moves_done++;
    return array;
}
