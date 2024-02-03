#pragma once

#include <string>
#include <cstdint>


std::string format_hex(const uint8_t *input, size_t n);
std::string format_hex(const std::string& input);
