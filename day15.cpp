#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <ranges>
#include <regex>
#include <set>
#include <sstream>
#include <utility>
#include <vector>

#include "utils.hpp"

static const std::map<char, std::pair<int, int>> directions = {
    {'v', {0, 1}}, {'>', {1, 0}}, {'^', {0, -1}}, {'<', {-1, 0}}};

auto parse(std::string_view input) {
  auto lines = getLines(input);
  auto split = std::find(lines.begin(), lines.end(), "");
  std::vector<std::string> map(lines.begin(), split);
  std::string instructions;
  for (++split; split != lines.end(); ++split) {
    instructions += *split;
  }

  return std::make_pair(map, instructions);
}

bool push(unsigned int x, unsigned int y, char d,
          std::vector<std::string> &map) {
  if (map[y][x] == '#') {
    return false;
  }

  if (map[y][x] == '.') {
    return true;
  }

  auto [dx, dy] = directions.at(d);

  if (push(x + dx, y + dy, d, map)) {
    std::swap(map[y][x], map[y + dy][x + dx]);
    return true;
  }

  return false;
}

bool widePush(const std::set<std::pair<unsigned, unsigned>> &current, char d,
              std::vector<std::string> &map) {
  if (current.empty()) {
    return true;
  }

  std::set<std::pair<unsigned, unsigned>> next;
  auto [dx, dy] = directions.at(d);
  for (auto [x, y] : current) {
    x = x + dx;
    y = y + dy;
    if (map[y][x] == '#') {
      return false;
    }

    if (map[y][x] == '.') {
      continue;
    }

    next.insert({x, y});
    if (d == 'v' || d == '^') {
      if (map[y][x] == ']') {
        next.insert({x - 1, y});
      } else if (map[y][x] == '[') {
        next.insert({x + 1, y});
      }
    }
  }

  if (widePush(next, d, map)) {
    for (auto [x, y] : current) {
      std::swap(map[y][x], map[y + dy][x + dx]);
    }
    return true;
  } else {
    return false;
  }
}

auto findRobot(std::vector<std::string> &map) {
  for (unsigned int y = 0; y < map.size(); ++y) {
    for (unsigned int x = 0; x < map[y].size(); ++x) {
      if (map[y][x] == '@') {
        return std::make_pair(x, y);
      }
    }
  }
  assert(false);  // never reached
}

auto calcSumGPS(const std::vector<std::string> &map, const char box = 'O') {
  uint64_t sum = 0;
  for (unsigned int y = 0; y < map.size(); ++y) {
    for (unsigned int x = 0; x < map[y].size(); ++x) {
      if (map[y][x] == box) {
        sum += x + 100 * y;
      }
    }
  }
  return sum;
}

auto calculate_1(std::string_view input) {
  auto [map, instructions] = parse(input);
  auto [x, y] = findRobot(map);

  printLines(map);

  for (auto d : instructions) {
    auto [dx, dy] = directions.at(d);
    if (push(x, y, d, map)) {
      x += dx;
      y += dy;
    }
  }
  printLines(map);

  auto result = calcSumGPS(map);

  return result;
}

static const std::map<char, std::string> transformation = {
    {'#', "##"}, {'O', "[]"}, {'.', ".."}, {'@', "@."}};

auto widen(std::vector<std::string> &map) {
  std::vector<std::string> result;
  for (auto &line : map) {
    std::string new_line;
    for (auto c : line) {
      new_line += transformation.at(c);
    }
    result.push_back(new_line);
  }

  return result;
}

auto calculate_2(std::string_view input) {
  auto [map, instructions] = parse(input);
  map = widen(map);
  auto [x, y] = findRobot(map);
  printLines(map);
  for (auto d : instructions) {
    // std::cout << "Direction: " << d << std::endl;
    auto [dx, dy] = directions.at(d);
    if (widePush({{x, y}}, d, map)) {
      x += dx;
      y += dy;
    }
    // printLines(map);
  }
  printLines(map);

  auto result = calcSumGPS(map, '[');

  return result;
}
int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "provide an input file" << std::endl;
    return 1;
  }

  auto input = readInput(argv[1]);
  if (!input) {
    std::cout << "could not read file: " << argv[1];
    return 1;
  }

  std::cout << "Solution 1: " << calculate_1(input.value()) << std::endl;
  std::cout << "Solution 2: " << calculate_2(input.value()) << std::endl;
}
