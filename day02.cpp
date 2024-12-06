#include <algorithm>
#include <iostream>
#include <map>
#include <ranges>
#include <regex>
#include <strstream>
#include <utility>
#include <vector>

#include "utils.hpp"

auto getDiffs(std::vector<int> report, int skip_index = -1) {
  auto filter_index =
      std::views::enumerate | std::views::filter([skip_index](auto val) {
        return std::get<0>(val) != skip_index;
      }) |
      std::views::transform([](auto val) { return std::get<1>(val); });

  auto _diffs = report | filter_index | std::views::slide(2) |
                std::views::transform([](auto vals) {
                  // TODO: This is soo uggly
                  auto v = vals.begin();
                  auto v1 = *v++;
                  auto v2 = *v;
                  return v1 - v2;
                });

  return std::vector<int>{_diffs.begin(), _diffs.end()};
}

int calculate_1(std::string_view input) {
  int result = 0;
  for (auto line : getLines(input)) {
    auto report = splitLine<int>(line);
    auto diffs = getDiffs(report);

    if (std::ranges::all_of(diffs, [](auto v) { return v < 0 && v > -4; }) ||
        std::ranges::all_of(diffs, [](auto v) { return v < 4 && v > 0; })) {
      result++;
    }
  }
  return result;
}

int calculate_2(std::string_view input) {
  int result = 0;
  for (auto line : getLines(input)) {
    auto report = splitLine<int>(line);
    for (int i = 0; i < report.size(); ++i) {
      auto diffs = getDiffs(report, i);

      if (std::ranges::all_of(diffs, [](auto v) { return v < 0 && v > -4; }) ||
          std::ranges::all_of(diffs, [](auto v) { return v < 4 && v > 0; })) {
        result++;
        break;
      }
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
