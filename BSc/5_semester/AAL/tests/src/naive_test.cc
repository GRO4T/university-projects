#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

#include "algorithm/generator.hpp"
#include "algorithm/naive.hpp"
#include "algorithm/property.hpp"
#include "algorithm/robot.hpp"

int main() {
    /* Robot robot(Generator::total_random(37)); */
    /* Robot robot("KYMCKYMCKYMCKYMCKYMC"); */
    Robot robot("KYMCKYMCKYMC"); 

    Naive naive_robot(robot, 0);
    std::cout << "result: " << PropertyParser(naive_robot.solve()).get_sorted_string() << "\n";
}
