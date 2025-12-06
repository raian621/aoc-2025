#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <string_view>
#include <unordered_set>

void print_usage(std::string_view name);
std::string_view get_solution_part_arg(int argc, const char **argv);

#endif
