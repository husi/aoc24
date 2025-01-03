#include <algorithm>
#include <iostream>
#include <map>
#include <ranges>
#include <regex>
#include <strstream>
#include <utility>
#include <vector>

#include "utils.hpp"

int calculate_1(std::string_view input) {
  int result = 0;
  std::regex mul_regex(R"-(mul\((\d{1,3}),(\d{1,3})\))-");
  std::regex_iterator it{input.begin(), input.end(), mul_regex};

  for (decltype(it) last{}; it != last; ++it) {
    auto v1 = stoi((*it)[1].str());
    auto v2 = stoi((*it)[2].str());
    result += v1 * v2;
  }

  return result;
}
int calculate_2(std::string_view input) {
  int result = 0;
  std::regex mul_regex(R"-(mul\((\d{1,3}),(\d{1,3})\)|do\(\)|don't\(\))-");
  std::regex_iterator it{input.begin(), input.end(), mul_regex};

  bool enabled = true;

  for (decltype(it) last{}; it != last; ++it) {
    if ("do()" == (*it)[0]) {
      enabled = true;
    } else if ("don't()" == (*it)[0]) {
      enabled = false;
    } else if (enabled) {
      auto v1 = stoi((*it)[1].str());
      auto v2 = stoi((*it)[2].str());
      result += v1 * v2;
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
