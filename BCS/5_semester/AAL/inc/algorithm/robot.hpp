#ifndef __STATE_HPP__
#define __STATE_HPP__

#include <string>

/*
 * Robot class managing move execution
 */
class Robot {
    std::string array;

public:
    Robot(const std::string& array) : array(array) {}

    operator std::string() const noexcept { return array; }
    const std::string& state() const noexcept { return array; }

    const std::string& move(std::size_t m);

    unsigned int get_moves_done() const noexcept { return moves_done; }

private:
    unsigned int moves_done = 0;
};

#endif
