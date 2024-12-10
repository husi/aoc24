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

struct Node {
  int id;
  unsigned int index;
  unsigned int lenght;
};

auto parse(std::string_view input) {
  unsigned id = 0;
  unsigned index = 0;
  auto pipeline =
      input |
      std::views::transform([](auto c) -> unsigned int { return c - '0'; }) |
      std::views::enumerate | std::views::transform([&id, &index](auto value) {
        auto [i, v] = value;
        Node result(i % 2 ? -1 : id++, index, v);
        index += v;
        return result;
      });

  return std::vector<Node>(pipeline.begin(), pipeline.end());
}

int calculate_1(std::string_view input) {
  int result = 0;
  auto nodes = parse(input);
  return result;
}

int calculate_2(std::string_view input) {
  int result = 0;
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
