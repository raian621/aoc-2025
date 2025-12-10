#!/usr/bin/env python3

from typing import List, TextIO
from sys import argv, stdin

def parse_positions(f: TextIO) -> List[tuple[int, int]]:
  red_tile_positions = []
  while True:
    line = f.readline()
    if not line:
      break
    dimensions = [int(s) for s in line[:-1].split(',')]
    if len(dimensions) == 2:
      red_tile_positions.append((dimensions[0], dimensions[1]))
  return red_tile_positions

def get_area(p1: tuple[int, int], p2: tuple[int, int]) -> int:
  (x1, y1) = p1
  (x2, y2) = p2
  return (abs(x1 - x2) + 1) * (abs(y1 - y2) + 1)

def solution1(positions: List[tuple[int, int]]) -> int:
  return max([get_area(positions[i], positions[j]) for i in range(len(positions)) for j in range(i+1, len(positions))])

def solution2(positions: List[tuple[int, int]]) -> int:
  return 0

def print_usage_and_exit():
  print(f"Usage: {argv[0]} [solution-part]")
  exit(1)

if len(argv) != 2 or argv[1] not in "12":
  print_usage_and_exit()

solution_part = int(argv[1])
print([solution1, solution2][solution_part-1](parse_positions(stdin)))