#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <ranges>
#include <regex>
#include <set>
#include <sstream>
#include <utility>
#include <vector>

#include "utils.hpp"

enum Direction {
  N = 0,
  E,
  S,
  W,

};

struct Guard {
  int x;
  int y;
  Direction direction;

  Guard(int x = 0, int y = 0) : x(x), y(y), direction(N) {}

  std::pair<int, int> vector() {
    switch (direction) {
      case N:
        return {0, -1};
      case E:
        return {1, 0};
      case S:
        return {0, 1};
      case W:
        return {-1, 0};
    }
  }

  auto front() {
    return std::make_pair(x + vector().first, y + vector().second);
  }

  void move() {
    x += vector().first;
    y += vector().second;
  }

  bool would_fall(size_t x_size, size_t y_size) {
    auto [nx, ny] = front();
    return nx < 0 || ny < 0 || nx >= x_size || ny >= y_size;
  }

  void turn_right() { direction = static_cast<Direction>((direction + 1) % 4); }
};

int calculate_1(std::string_view input) {
  int result = 1;
  auto map = getLines(input);

  Guard g;

  for (int i = 0; i < map.size(); ++i) {
    if (auto gindex = map[i].find('^'); gindex != std::string::npos) {
      g = Guard(gindex, i);
      map[i][gindex] = 'X';
    }
  }

  while (!g.would_fall(map[0].size(), map.size())) {
    auto [fx, fy] = g.front();
    switch (map[fy][fx]) {
      case '#':
        g.turn_right();
        break;
      case '.':
        result++;
        map[fy][fx] = 'X';
      case 'X':
        g.move();
        break;
      default:
        assert(false);
    }
    // printLines(map);
  }
  return result;
}

int calculate_2(std::string_view input) {
  int result = 0;
  auto map = getLines(input);

  Guard g;

  for (int i = 0; i < map.size(); ++i) {
    if (auto gindex = map[i].find('^'); gindex != std::string::npos) {
      g = Guard(gindex, i);
      map[i][gindex] = 'X';
    }
  }

  std::set<std::pair<int, int>> possible_obstacles;

  Guard origg = g;

  while (!g.would_fall(map[0].size(), map.size())) {
    auto [fx, fy] = g.front();
    switch (map[fy][fx]) {
      case '#':
        g.turn_right();
        break;
      case '.':
        possible_obstacles.insert(std::make_pair(fx, fy));
      case 'X':
        g.move();
        break;
      default:
        assert(false);
    }
  }

  std::cout << std::endl
            << "Possible obstackles: " << possible_obstacles.size()
            << std::endl;

  int iteration = 0;
  for (auto obstacle : possible_obstacles) {
    std::map<std::pair<int, int>, std::set<Direction>> visited_map;
    if (++iteration % 100 == 0) {
      std::cout << "iteration: " << iteration << std::endl;
    }
    map[obstacle.second][obstacle.first] = '#';
    g = origg;  // reset the guard
    while (!g.would_fall(map[0].size(), map.size())) {
      visited_map[{g.x, g.y}].insert(g.direction);
      auto [fx, fy] = g.front();
      switch (map[fy][fx]) {
        case '#':
          g.turn_right();
          break;
        case '.':
        case 'X':
          g.move();
          break;
        default:
          assert(false);
      }
      if (visited_map[{g.x, g.y}].contains(g.direction)) {
        // we were here already
        result++;
        break;
      }
    }

    map[obstacle.second][obstacle.first] = '.';
  }

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
