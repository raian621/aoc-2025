#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "common.h"

struct Position; // forward decl

typedef uint64_t (*solution_fn)(const std::vector<Position> &);

struct Position {
  uint64_t x, y, z;

  float distance(const Position &other) const {
    float dx = (int64_t)other.x - (int64_t)x;
    float dy = (int64_t)other.y - (int64_t)y;
    float dz = (int64_t)other.z - (int64_t)z;
    return std::sqrt((float)dx * dx + (float)dy * dy + (float)dz * dz);
  }
};

struct Edge {
  size_t u, v;
  float weight;
};

std::ostream &operator<<(std::ostream &out, const Position &p) {
  return out << "Position(" << p.x << ", " << p.y << ", " << p.z << ")\n";
}

std::ostream &operator<<(std::ostream &out, const Edge &e) {
  return out << "Edge(" << e.u << ", " << e.v << ", " << e.weight << ")\n";
}

std::vector<Edge> generate_edges(const std::vector<Position> &positions) {
  std::vector<Edge> edges;

  for (size_t i = 0; i < positions.size(); i++) {
    for (size_t j = i + 1; j < positions.size(); j++) {
      edges.push_back(
          {.u = i, .v = j, .weight = positions[i].distance(positions[j])});
    }
  }

  return edges;
}

size_t find(const size_t x, std::unordered_map<size_t, size_t> &parent,
            std::unordered_map<size_t, size_t> &rank) {
  if (const auto &x_root = parent.find(x); x_root == parent.end()) {
    parent[x] = x;
    rank[x] = 1;
    return x;
  }

  const size_t root = parent[x];
  if (root != parent[root]) {
    parent[x] = find(root, parent, rank);
    return parent[x];
  }

  return root;
}

void unite(const size_t u, const size_t v,
           std::unordered_map<size_t, size_t> &parent,
           std::unordered_map<size_t, size_t> &rank) {
  const size_t u_root = find(u, parent, rank);
  const size_t v_root = find(v, parent, rank);
  if (u_root == v_root) {
    return;
  }
  const size_t u_rank = rank[u_root];
  const size_t v_rank = rank[v_root];

  if (u_rank > v_rank) {
    rank[u_root] += v_rank;
    parent[v_root] = u_root;
  } else {
    rank[v_root] += u_rank;
    parent[u_root] = v_root;
  }
}

std::vector<Edge> get_smallest_edges(const std::vector<Position> &positions,
                                     size_t n) {
  std::vector<Edge> edges = generate_edges(positions);
  std::partial_sort(
      edges.begin(), edges.begin() + n, edges.end(),
      [](const Edge &a, const Edge &b) { return a.weight < b.weight; });
  edges.resize(n);
  return edges;
}

uint64_t solution1(const std::vector<Position> &positions) {
  std::vector<Edge> edges = get_smallest_edges(positions, 1000);
  std::unordered_map<size_t, size_t> parent;
  std::unordered_map<size_t, size_t> rank;

  for (const Edge &e : edges) {
    unite(e.u, e.v, parent, rank);
  }

  std::vector<uint64_t> sizes;
  for (const auto [s, size] : rank) {
    // filter for root nodes
    if (find(s, parent, rank) == s) {
      sizes.push_back(size);
    }
  }

  std::partial_sort(sizes.begin(), sizes.begin() + 3, sizes.end(),
                    std::greater<uint64_t>());

  return std::reduce(
      sizes.begin(), sizes.begin() + 3, (size_t)1,
      [](const uint64_t prod, const uint64_t val) { return prod * val; });
}

uint64_t solution2(const std::vector<Position> &positions) {
  std::vector<Edge> edges = generate_edges(positions);
  std::sort(edges.begin(), edges.end(),
            [](const Edge &a, const Edge &b) { return a.weight < b.weight; });
  std::unordered_map<size_t, size_t> parent;
  std::unordered_map<size_t, size_t> rank;
  bool fully_connected = true;
  size_t root = 0;

  for (const Edge &e : edges) {
    unite(e.u, e.v, parent, rank);
    root = find(0, parent, rank);
    fully_connected = true;
    for (size_t i = 1; fully_connected && i < positions.size(); i++) {
      fully_connected = find(i, parent, rank) == root;
    }
    if (fully_connected) {
      return positions[e.u].x * positions[e.v].x;
    }
  }

  return 0;
}

Position parse_position(const std::string &line) {
  size_t y_offset = line.find(',') + 1;
  size_t z_offset = line.find(',', y_offset) + 1;
  return Position{
      .x = std::stoull(line, 0, 10),
      .y = std::stoull(line.substr(y_offset, z_offset - y_offset), 0, 10),
      .z = std::stoull(line.substr(z_offset), 0, 10)};
}

std::vector<Position> parse_positions(std::istream &in) {
  std::string line;
  std::vector<Position> positions;
  while (std::getline(in, line, '\n')) {
    positions.push_back(parse_position(line));
  }
  return positions;
}

int main(int argc, const char **argv) {
  const std::string_view solution_part = get_solution_part_arg(argc, argv);
  const std::vector<Position> positions = parse_positions(std::cin);
  std::cout << std::unordered_map<std::string_view,
                                  solution_fn>{{"1", solution1},
                                               {"2", solution2}}[solution_part](
                   positions)
            << std::endl;
}