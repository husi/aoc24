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

void process(std::list<uint64_t> &stones) {
  auto it = stones.begin();

  while (it != stones.end()) {
    if (0 == *it) {
      *it = 1;
    } else if (int length = std::trunc(std::log10(*it)) + 1; length % 2 == 0) {
      int divisor = std::pow(10, length / 2);
      auto s2 = *it % divisor;
      *(it++) /= divisor;
      it = stones.insert(it, s2);
    } else {
      *it *= 2024;
    }
    ++it;
  }
};

uint64_t processStone(uint64_t stone, int steps) {
  static std::map<std::pair<uint64_t, int>, uint64_t> cache;
  static uint64_t cacheHits = 0;

  if (steps == 0) {
    return 1;
  }

  if (auto it = cache.find({stone, steps}); it != cache.end()) {
    return it->second;
  }

  uint64_t result = 0;

  if (0 == stone) {
    result = processStone(1, steps - 1);
  } else if (int length = std::trunc(std::log10(stone)) + 1; length % 2 == 0) {
    int divisor = std::pow(10, length / 2);
    result = processStone(stone / divisor, steps - 1) +
             processStone(stone % divisor, steps - 1);
  } else {
    result = processStone(stone * 2024, steps - 1);
  }

  cache[{stone, steps}] = result;
  return result;
}

auto calculate_deep(std::string_view input, int steps) {
  auto v = splitLine<uint64_t>(input);
  auto stones = std::list(v.begin(), v.end());

  auto result = stones | std::views::transform([steps](auto stone) {
                  auto result = processStone(stone, steps);
                  std::cout << stone << " >>>> " << result << std::endl;
                  return result;
                });

  return std::accumulate(result.begin(), result.end(), uint64_t(0));
}

auto calculate_wide(std::string_view input, int steps) {
  auto v = splitLine<uint64_t>(input);
  auto stones = std::list(v.begin(), v.end());

  for (int i = 0; i < steps; ++i) {
    process(stones);
  }

  return stones.size();
}

auto calculate_1(std::string_view input) { return calculate_deep(input, 25); }

auto calculate_2(std::string_view input) { return calculate_deep(input, 75); }

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
