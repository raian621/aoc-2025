#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "common.h"

using Range = ::std::pair<uint64_t, uint64_t>;

bool is_fresh_ingredient(const std::vector<Range> &id_ranges, uint64_t id) {
  size_t lo = 0;
  size_t hi = id_ranges.size() - 1;
  size_t mid = (lo + hi) / 2;
  Range r = id_ranges[mid];

  while (lo < hi) {
    if (r.first <= id && id <= r.second) {
      return true;
    } else if (r.first > id) {
      hi = mid;
    } else {
      lo = mid + 1;
    }

    mid = (lo + hi) / 2;
    r = id_ranges[mid];
  }

  return r.first <= id && id <= r.second;
}

std::vector<Range> get_compact_id_ranges(const std::vector<Range> id_ranges) {
  std::vector<Range> sorted_id_ranges(id_ranges);
  std::sort(sorted_id_ranges.begin(), sorted_id_ranges.end(),
            [](const Range &a, const Range &b) { return a.first < b.first; });
  std::vector<Range> compact_id_ranges;
  Range curr_range = sorted_id_ranges.front();
  Range next_range;

  for (size_t i = 1; i < id_ranges.size(); ++i) {
    next_range = sorted_id_ranges[i];
    if (curr_range.first <= next_range.first &&
        next_range.first <= curr_range.second + 1) {
      curr_range.second = std::max(curr_range.second, next_range.second);
      continue;
    }
    compact_id_ranges.push_back(curr_range);
    curr_range = std::move(next_range);
  }
  compact_id_ranges.push_back(curr_range);
  for (int i = 1; i < compact_id_ranges.size(); i++) {
    Range r1 = compact_id_ranges[i - 1], r2 = compact_id_ranges[i];
    if (r1.second > r2.second) {
      exit(1);
    }
  }

  return compact_id_ranges;
}

uint64_t solution1(const std::vector<Range> &id_ranges,
              const std::vector<uint64_t> &ingredient_ids) {
  const std::vector<Range> id_ranges_compact = get_compact_id_ranges(id_ranges);
  int fresh_ingredient_count = 0;

  for (const uint64_t id : ingredient_ids) {
    if (is_fresh_ingredient(id_ranges_compact, id)) {
      fresh_ingredient_count++;
    }
  }

  return fresh_ingredient_count;
}

uint64_t solution2(const std::vector<Range> &id_ranges) {
  const std::vector<Range> id_ranges_compact = get_compact_id_ranges(id_ranges);
  uint64_t fresh_ingredient_count = 0;

  for (const Range &r : id_ranges_compact) {
    fresh_ingredient_count += r.second - r.first + 1;
  }

  return fresh_ingredient_count;
}

Range parse_range(const std::string line) {
  size_t dash_idx;
  uint64_t start = std::stoull(line, &dash_idx, 10);
  uint64_t end = std::stoull(line.substr(dash_idx + 1), 0, 10);
  return std::make_pair(start, end);
}

void parse_input(std::istream &in, std::vector<Range> &id_ranges_output,
                 std::vector<uint64_t> &ingredient_ids_output) {
  std::string line;
  while (std::getline(in, line, '\n') && !line.empty()) {
    id_ranges_output.push_back(parse_range(line));
  }
  while (std::getline(in, line, '\n')) {
    ingredient_ids_output.push_back(std::stoull(line, 0, 10));
  }
}

uint64_t get_solution(std::string_view solution_part) {
  std::vector<Range> id_ranges;
  std::vector<uint64_t> ingredient_ids;
  parse_input(std::cin, id_ranges, ingredient_ids);

  if (solution_part == "1") {
    return solution1(id_ranges, ingredient_ids);
  } else if (solution_part == "2") {
    return solution2(id_ranges);
  }

  return 0;
}

int main(int argc, const char **argv) {
  std::string_view solution_part = get_solution_part_arg(argc, argv);
  std::cout << get_solution(solution_part) << std::endl;
}