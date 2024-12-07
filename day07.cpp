#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <map>
#include <ranges>
#include <regex>
#include <set>
#include <sstream>
#include <utility>
#include <vector>

#include "utils.hpp"

auto parse(std::string_view input) {
  auto lines = getLines(input);

  std::vector<std::pair<unsigned long long, std::vector<unsigned>>> result;
  for (auto line : lines) {
    auto index = line.find(':');
    auto target = atoll(line.substr(0, index).c_str());
    auto values = splitLine<unsigned>(line.substr(index + 1));

    result.push_back({target, values});
  }

  return result;
}

bool solvable(unsigned long long target, size_t index,
              std::vector<unsigned> values) {
  auto value = values[index];

  if (index == 0) {
    return target == value;
  }

  if (target % value == 0) {
    if (solvable(target / value, index - 1, values)) {
      return true;
    }
  }
  if (target > value) {
    if (solvable(target - value, index - 1, values)) {
      return true;
    }
  }
  return false;
}

bool solvable2(unsigned long long target, size_t index,
               std::vector<unsigned> values) {
  auto value = values[index];

  if (index == 0) {
    return target == value;
  }

  int length = std::trunc(std::log10(value)) + 1;
  int divisor = std::pow(10, length);
  if (target % divisor == value) {
    if (solvable2(target / divisor, index - 1, values)) {
      return true;
    }
  }
  if (target % value == 0) {
    if (solvable2(target / value, index - 1, values)) {
      return true;
    }
  }
  if (target > value) {
    if (solvable2(target - value, index - 1, values)) {
      return true;
    }
  }
  return false;
}

auto calculate_1(std::string_view input) {
  int64_t result = 0;
  for (auto [target, values] : parse(input)) {
    if (solvable(target, values.size() - 1, values)) {
      result += target;
    }
  }

  return result;
}

auto calculate_2(std::string_view input) {
  int64_t result = 0;
  for (auto [target, values] : parse(input)) {
    if (solvable2(target, values.size() - 1, values)) {
      result += target;
    }
  }

  return result;
}

int main(int argc, char** argv) {
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
