#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <numeric>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "common.h"

struct Problem {
  std::vector<uint64_t> values;
  char operation;
};

void load_values_into_problems(const std::string &line,
                               std::vector<Problem> &problems_output) {
  size_t global_offset = 0;
  size_t local_offset = 0;
  uint64_t value;
  std::stringstream stream(line);

  if (problems_output.size() == 0) {
    while (stream >> value) {
      problems_output.push_back(Problem{.values = {value}, .operation = '?'});
    }
  } else {
    for (int i = 0; stream >> value; i++) {
      problems_output[i].values.push_back(value);
    }
  }
}

void load_operations_into_problems(const std::string &line,
                                   std::vector<Problem> &problems_output) {
  auto problems_it = problems_output.begin();
  auto line_it = line.begin();
  while (line_it != line.end() && problems_it != problems_output.end()) {
    if (*line_it == '+' || *line_it == '*') {
      problems_it->operation = *line_it;
      problems_it++;
    }
    line_it++;
  }
}

std::vector<Problem> parse_problems(std::istream &in) {
  std::vector<Problem> problems;
  std::string line;
  bool is_value_line = false;
  while (std::getline(in, line, '\n')) {
    auto it = std::find_if(line.begin(), line.end(),
                           [](const char c) { return !std::isspace(c); });
    if (std::isdigit(*it)) {
      load_values_into_problems(line, problems);
    } else {
      load_operations_into_problems(line, problems);
    }
  }
  return problems;
}

std::vector<std::string>
transpose_lines(const std::vector<std::string> &lines) {
  std::vector<std::string> transposed_lines;
  std::stringstream stream;

  for (int i = 0; i < lines[0].length(); i++) {
    for (int j = 0; j < lines.size(); j++) {
      stream.put(lines[j][i]);
    }
    transposed_lines.push_back(stream.str());
    stream = std::stringstream();
  }

  return transposed_lines;
}

std::vector<Problem> parse_problems_with_transposed_numbers(std::istream &in) {
  std::vector<std::string> lines;
  std::string line;

  while (std::getline(in, line, '\n')) {
    lines.push_back(line);
  }

  const std::vector<std::string> transposed_lines =
      transpose_lines(std::move(lines));
  std::vector<Problem> problems;
  Problem curr;
  for (std::string line : transposed_lines) {
    if (std::find_if(line.begin(), line.end(), [](const char c) {
          return !std::isspace(c);
        }) == line.end()) {
      if (!curr.values.empty()) {
        problems.push_back(curr);
        curr = Problem{};
      }
      continue;
    }
    curr.values.push_back(std::stoi(line, 0, 10));
    if (line.back() == '*' || line.back() == '+') {
      curr.operation = line.back();
    }
  }
  if (!curr.values.empty()) {
    problems.push_back(curr);
    curr = Problem{};
  }

  return problems;
}

uint64_t solution1(const std::vector<Problem> &problems) {
  return std::transform_reduce(
      problems.begin(), problems.end(), (uint64_t)0, std::plus{},
      [](const Problem &p) {
        uint64_t result = 0;
        if (p.operation == '*') {
          result = std::reduce(p.values.begin(), p.values.end(), (uint64_t)1,
                               std::multiplies{});
        } else if (p.operation == '+') {
          result =
              std::reduce(p.values.begin(), p.values.end(), 0, std::plus{});
        }

        return result;
      });
}

int main(int argc, const char **argv) {
  std::string_view solution_part = get_solution_part_arg(argc, argv);
  uint64_t solution = 0;
  if (solution_part == "1") {
    solution = solution1(parse_problems(std::cin));
  } else {
    solution = solution1(parse_problems_with_transposed_numbers(std::cin));
  }
  std::cout << solution << std::endl;
}