#!/usr/bin/env lua

NONE = 0
BEAM = 1

--- @param f file*
--- @return table<integer, string>
function ParseField(f)
  local lines = {}
  for line in f:lines("l") do
    lines[#lines + 1] = line
  end
  return lines
end

--- @param field table<integer, string>
--- @return integer
function Solution1(field)
  local beam_split_count = 0
  local prev_beam_states = {}
  local curr_beam_states = {}
  local line_length = field[1]:len()
  local c = '.'
  for i = 1, line_length do
    prev_beam_states[i] = NONE
  end
  for _, line in pairs(field) do
    for i = 1, line_length do
      c = line:sub(i, i)
      if c == 'S' then
        curr_beam_states[i] = BEAM
      elseif c == '^' and prev_beam_states[i] == BEAM then
        if i > 1 then
          curr_beam_states[i - 1] = BEAM
        end
        curr_beam_states[i] = NONE
        if i < line:len() then
          curr_beam_states[i + 1] = BEAM
        end
        beam_split_count = beam_split_count + 1
      else
        curr_beam_states[i] = prev_beam_states[i]
      end
    end
    prev_beam_states = curr_beam_states
  end
  return beam_split_count
end

--- @param field table<integer, string>
--- @param row integer
--- @param col integer
--- @param memo table<table, integer>
--- @return integer
function MemoizedSolution2(field, row, col, memo)
  if col > field[1]:len() or col < 1 then
    return 0
  end
  if row >= #field then
    return 1
  end
  if memo[row][col] == nil then
    if field[row + 1]:sub(col, col) == '^' then
      memo[row][col] = MemoizedSolution2(field, row + 1, col + 1, memo) +
      MemoizedSolution2(field, row + 1, col - 1, memo)
    else
      memo[row][col] = MemoizedSolution2(field, row + 1, col, memo)
    end
  end

  return memo[row][col]
end

--- @param field table<integer, string>
--- @return integer
function Solution2(field)
  local first_line = field[1]
  local source_index = first_line:find("S")
  if source_index == nil then
    error("source of beam not found")
  end
  local memo = {}
  for i = 1, #field do
    memo[i] = {}
  end
  return MemoizedSolution2(field, 1, source_index, memo)
end

function PrintUsage()
  print("Usage: ", arg[0], " [solution-part]")
end

function Main()
  if #arg ~= 1 then
    PrintUsage()
    os.exit(1)
  end
  local solution_part = arg[1]
  local solutions = { ["1"] = Solution1, ["2"] = Solution2 }
  print(solutions[solution_part](ParseField(io.stdin)))
end

Main()
