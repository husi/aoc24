#include <algorithm>
#include <cassert>
#include <iostream>
#include <list>
#include <map>
#include <ranges>
#include <regex>
#include <set>
#include <sstream>
#include <utility>
#include <vector>

#include "utils.hpp"

struct Point {
  unsigned int x;
  unsigned int y;
  char height;
  int numRoutes = -1;

  bool operator<(const Point &other) const {
    return std::tie(x, y) < std::tie(other.x, other.y);
  }
};

struct Map {
  std::vector<std::vector<Point>> map;

  std::optional<Point> get(unsigned int x, unsigned int y) {
    if (x < 0 || y < 0 || y >= map.size() || x >= map[0].size()) {
      return {};
    }
    return map[y][x];
  }

  std::vector<Point> neighbours(const Point &p) {
    std::vector<Point> neighbours;
    for (auto [x, y] : std::vector<std::pair<unsigned, unsigned>>{
             {p.x - 1, p.y}, {p.x + 1, p.y}, {p.x, p.y - 1}, {p.x, p.y + 1}}) {
      if (auto p = get(x, y)) {
        neighbours.push_back(p.value());
      }
    }

    return neighbours;
  }

  static Map parse(std::string_view input) {
    auto parsed =
        getLines(input) | std::views::enumerate |
        std::views::transform([](auto line) {
          auto [y, linestr] = line;
          auto parsedLine = linestr | std::views::enumerate |
                            std::views::transform([y](auto point) {
                              auto [x, height] = point;
                              char h = static_cast<char>(height) - '0';
                              return Point{static_cast<unsigned>(x),
                                           static_cast<unsigned>(y), h};
                            });
          return std::vector<Point>(parsedLine.begin(), parsedLine.end());
        });
    return {{parsed.begin(), parsed.end()}};
  }

  std::set<Point> findPeeks(Point &p) {
    if (p.height == 9) {
      return {p};
    }

    std::set<Point> result;
    for (auto n : neighbours(p)) {
      if (n.height != p.height + 1) continue;

      auto peeks = findPeeks(n);
      result.insert(peeks.begin(), peeks.end());
    }
    return result;
  }

  unsigned int calculateRoutes(Point &p) {
    if (p.numRoutes >= 0) {
      return p.numRoutes;
    }

    unsigned int routes = 0;
    if (p.height == 9) {
      routes = 1;
    } else {
      for (auto n : neighbours(p)) {
        if (n.height == p.height + 1) {
          routes += calculateRoutes(n);
        }
      }
    }
    p.numRoutes = routes;
    return routes;
  }
};

auto calculate_1(std::string_view input) {
  unsigned result = 0;
  auto map = Map::parse(input);

  for (auto p : map.map | std::views::join | std::views::filter([](auto point) {
                  return point.height == 0;
                })) {
    auto peeks = map.findPeeks(p);
    result += peeks.size();
  }

  return result;
}

auto calculate_2(std::string_view input) {
  unsigned result = 0;
  auto map = Map::parse(input);

  for (auto p : map.map | std::views::join | std::views::filter([](auto point) {
                  return point.height == 0;
                })) {
    result += map.calculateRoutes(p);
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
