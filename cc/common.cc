#include <iostream>
#include <unordered_set>

void print_usage(std::string_view name) {
  std::cout << "Usage: " << name << " <solution-part>" << std::endl;
}

std::string_view get_solution_part_arg(int argc, const char **argv) {
  if (argc != 2) {
    print_usage(argv[0]);
    exit(1);
  }

  std::string_view solution_part = argv[1];
  const std::unordered_set<std::string_view> accepted_solution_part_vals = {
      "1",
      "2",
  };

  if (const auto it = accepted_solution_part_vals.find(solution_part);
      it == accepted_solution_part_vals.end()) {
    print_usage(argv[0]);
    exit(1);
  }

  return solution_part;
}