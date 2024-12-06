#include <algorithm>
#include <iostream>
#include <map>
#include <ranges>
#include <strstream>
#include <utility>
#include <vector>

#include "utils.hpp"

auto getLists(std::string_view input) {
  std::vector<int> l1, l2;

  std::istrstream stream{input.data()};

  while (!stream.eof()) {
    int i1, i2;
    stream >> i1 >> i2;

    l1.emplace_back(i1);
    l2.emplace_back(i2);
  }

  return std::make_pair(l1, l2);
}

int calculate_1(std::string_view input) {
  auto [l1, l2] = getLists(input);

  std::ranges::sort(l1);
  std::ranges::sort(l2);

  auto zip = std::ranges::zip_view{l1, l2};

  auto sum = std::ranges::fold_left(zip, 0, [](int s, auto val) {
    auto [i1, i2] = val;
    return s + abs(i1 - i2);
  });

  return sum;
}

int calculate_2(std::string_view input) {
  auto [l1, l2] = getLists(input);

  std::map<int, int> m1;
  for (auto v : l1) {
    m1[v]++;
  }

  auto sum = std::ranges::fold_left(
      l2, 0, [&m1](int s, auto val) { return s + val * m1[val]; });

  return sum;
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
