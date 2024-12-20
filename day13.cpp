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

struct Task {
  std::pair<int64_t, int64_t> buttonA;
  std::pair<int64_t, int64_t> buttonB;
  std::pair<int64_t, int64_t> target;
};

std::vector<Task> parse(std::string_view input) {
  static std::regex regex(
      R"-(Button A: X\+(\d+), Y\+(\d+)\nButton B: X\+(\d+), Y\+(\d+)\nPrize: X=(\d+), Y=(\d+))-");

  auto found = std::regex_search(input.data(), regex);

  auto it = std::cregex_iterator(input.begin(), input.end(), regex);
  std::vector<Task> result;
  for (decltype(it) last; it != last; ++it) {
    result.emplace_back(
        std::make_pair(atoi(it->str(1).data()), atoi(it->str(2).data())),
        std::make_pair(atoi(it->str(3).data()), atoi(it->str(4).data())),
        std::make_pair(atoi(it->str(5).data()), atoi(it->str(6).data())));
  }

  return result;
}

int64_t solve_with_cache(const std::pair<int64_t, int64_t> &moveA,
                         const std::pair<int64_t, int64_t> &moveB,
                         const std::pair<int64_t, int64_t> &target,
                         const std::pair<int64_t, int64_t> &presses,
                         std::map<std::pair<int64_t, int64_t>, int64_t> &cache);

int64_t solve(const std::pair<int64_t, int64_t> &moveA,
              const std::pair<int64_t, int64_t> &moveB,
              const std::pair<int64_t, int64_t> &target,
              const std::pair<int64_t, int64_t> &presses,
              std::map<std::pair<int64_t, int64_t>, int64_t> &cache) {
  if (auto it = cache.find(target); it != cache.end()) {
    return it->second;
  }

  if ((moveA.first + moveB.first) * 100 < target.first ||
      (moveA.second + moveB.second) * 100 < target.second) {
    return -1;
  }

  if (presses.first > 100 || presses.second > 100) {
    return -1;
  }

  if (target.first < 0 || target.second < 0) {
    return -1;
  }

  if (target == moveA) {
    return 3;
  }

  if (target == moveB) {
    return 1;
  }

  auto withA = solve_with_cache(
      moveA, moveB, {target.first - moveA.first, target.second - moveA.second},
      {presses.first + 1, presses.second}, cache);
  auto withB = solve_with_cache(
      moveA, moveB, {target.first - moveB.first, target.second - moveB.second},
      {presses.first, presses.second + 1}, cache);

  if (withA == -1 && withB == -1) {
    return -1;
  }

  if (withA == -1) {
    return withB + 1;
  } else {
    return withA + 3;
  }
}

int64_t solve_with_cache(
    const std::pair<int64_t, int64_t> &moveA,
    const std::pair<int64_t, int64_t> &moveB,
    const std::pair<int64_t, int64_t> &target,
    const std::pair<int64_t, int64_t> &presses,
    std::map<std::pair<int64_t, int64_t>, int64_t> &cache) {
  auto result = solve(moveA, moveB, target, presses, cache);
  cache[target] = result;

  return result;
}

int64_t attempt2(const std::pair<int64_t, int64_t> &moveA,
                 const std::pair<int64_t, int64_t> &moveB,
                 const std::pair<int64_t, int64_t> &target,
                 const std::pair<int64_t, int64_t> &presses,
                 std::map<std::pair<int64_t, int64_t>, int64_t> &cache) {
  auto bbb = std::make_pair(3 * moveB.first, 3 * moveB.second);
  auto cheapMove = target.first - moveA.first + target.second - moveA.second >
                           target.first - bbb.first + target.second - bbb.second
                       ? moveA
                       : moveB;
  auto badMove = cheapMove == moveA ? moveB : moveA;

  auto divx = std::div(target.first, cheapMove.first);
  auto divy = std::div(target.second, cheapMove.second);

  auto divisor = std::min(divx.quot, divy.quot);
  auto tempTarget = std::make_pair(target.first - divisor * cheapMove.first,
                                   target.second - divisor * cheapMove.second);

  auto result = -1;
  while (divisor >= 0) {
    auto bdivx = std::div(tempTarget.first, badMove.first);
    auto bdivy = std::div(tempTarget.second, badMove.second);

    if (bdivx.rem == 0 && bdivy.rem == 0 && bdivx.quot == bdivy.quot) {
      return cheapMove == moveA ? 3 * divisor + bdivx.quot
                                : divisor + 3 * bdivx.quot;
    }

    divisor--;
    if (divisor % 1000 == 0) {
      std::cout << "divisor: " << divisor
                << "  temptarget: " << tempTarget.first << ", "
                << tempTarget.second << std::endl;
    }
    tempTarget = std::make_pair(tempTarget.first + cheapMove.first,
                                tempTarget.second + cheapMove.second);
  }

  return -1;
}

int64_t with_binary_search(const std::pair<int64_t, int64_t> &moveA,
                           const std::pair<int64_t, int64_t> &moveB,
                           const std::pair<int64_t, int64_t> &target) {
  auto bbb = std::make_pair(3 * moveB.first, 3 * moveB.second);
  auto cheapMove = target.first - moveA.first + target.second - moveA.second >
                           target.first - bbb.first + target.second - bbb.second
                       ? moveA
                       : moveB;
  auto badMove = cheapMove == moveA ? moveB : moveA;

  auto divx = std::div(target.first, cheapMove.first);
  auto divy = std::div(target.second, cheapMove.second);

  auto maxcheap = std::min(divx.quot, divy.quot);
}

auto calculate_1(std::string_view input) {
  int64_t result = 0;
  auto tasks = parse(input);

  for (auto task : tasks) {
    std::map<std::pair<int64_t, int64_t>, int64_t> cache;
    auto r = attempt2(task.buttonA, task.buttonB, task.target, {0, 0}, cache);
    if (r > 0) {
      result += r;
      std::cout << "result: " << result << std::endl;
    }
  }

  return result;
}

auto calculate_2(std::string_view input) {
  int64_t result = 0;
  auto tasks = parse(input);

  for (auto task : tasks) {
    std::map<std::pair<int64_t, int64_t>, int64_t> cache;
    auto r = attempt2(task.buttonA, task.buttonB,
                      {task.target.first + 10000000000000,
                       task.target.second + 10000000000000},
                      {0, 0}, cache);
    if (r > 0) {
      result += r;
    }
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
