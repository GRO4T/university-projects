#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"

unsigned int Factorial(unsigned int number) { return number <= 1 ? number : Factorial(number - 1) * number; }

