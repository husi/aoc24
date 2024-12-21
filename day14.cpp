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

struct Robot {
  std::pair<int, int> position;
  std::pair<int, int> velocity;

  void move(int seconds) {
    position.first += velocity.first * seconds;
    position.second += velocity.second * seconds;
  }

  void wrap(unsigned int width, unsigned int height) {
    auto posx = std::div(position.first, width).rem;
    auto posy = std::div(position.second, height).rem;
    position.first = posx < 0 ? width + posx : posx;
    position.second = posy < 0 ? height + posy : posy;
  }
};

struct Board {
  unsigned int width;
  unsigned int height;

  std::vector<Robot> robots;

  void move(int seconds) {
    for (auto &robot : robots) {
      robot.move(seconds);
      robot.wrap(width, height);
    }
  }

  auto calcQuadrants() {
    std::map<std::pair<unsigned int, unsigned int>, std::vector<Robot>>
        quadrants;
    for (auto robot : robots) {
      auto xsplit = width / 2;
      auto ysplit = height / 2;

      if (robot.position.first == xsplit || robot.position.second == ysplit) {
        continue;
      }

      auto qx = robot.position.first < width / 2 ? 0 : 1;
      auto qy = robot.position.second < height / 2 ? 0 : 1;

      quadrants[{qx, qy}].push_back(robot);
    }
    return quadrants;
  }
  void draw(std::ostream &os = std::cout) {
    std::vector<std::vector<char>> board(height, std::vector<char>(width, '.'));
    for (auto robot : robots) {
      board[robot.position.second][robot.position.first] = '#';
    }

    for (auto row : board) {
      for (auto cell : row) {
        os << cell;
      }
      os << std::endl;
    }
  }
};

Board parse(std::string_view input) {
  static std::regex regex(R"-(=(-?\d+),(-?\d+) v=(-?\d+),(-?\d+))-");

  auto start = input.find('\n');
  auto coma = input.find(',');
  unsigned int width = atoi(input.substr(0, coma).data());
  unsigned int height = atoi(input.substr(coma + 1, start).data());

  auto it = std::cregex_iterator(input.begin() + start, input.end(), regex);
  std::vector<Robot> robots;
  for (decltype(it) last; it != last; ++it) {
    robots.emplace_back(
        std::make_pair(atoi(it->str(1).data()), atoi(it->str(2).data())),
        std::make_pair(atoi(it->str(3).data()), atoi(it->str(4).data())));
  }

  return Board{width, height, robots};
}

auto calculate_1(std::string_view input) {
  auto robots = parse(input);

  robots.move(100);
  auto quadrants = robots.calcQuadrants();

  return std::accumulate(
      quadrants.begin(), quadrants.end(), 1,
      [](auto acc, auto q) { return acc * q.second.size(); });
}

auto calculate_2(std::string_view input) {
  auto robots = parse(input);

  int i = 1;
  while (true) {
    if (i % 1000000 == 0) {
      std::cout << "Seconds: " << i << std::endl;
    }

    robots.move(1);
    std::stringstream ss;
    robots.draw(ss);

    // This was not my idea, I saw it in the subreddit and wanted to see the
    // tree
    if (ss.str().find("###########") != std::string::npos) {
      robots.draw();
      std::cout << "Seconds: " << i << std::endl;
      return 0;
    }

    i++;
  }

  return 0;
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
