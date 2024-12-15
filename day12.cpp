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

struct Region {
  std::set<std::pair<int, int>> plants;
  unsigned int area;
  unsigned int perimeter;
};

class RegionMap {
  std::map<char, std::list<std::unique_ptr<Region>>> regions;

 public:
  void add(char letter, int x, int y) {
    auto &regionList = regions[letter];
    auto it1 = std::find_if(regionList.begin(), regionList.end(),
                            [x, y](const auto &region) {
                              return region->plants.contains({x - 1, y});
                            });
    auto it2 = std::find_if(regionList.begin(), regionList.end(),
                            [x, y](const auto &region) {
                              return region->plants.contains({x, y - 1});
                            });

    // This is a non joint region
    if (it1 == regionList.end() && it2 == regionList.end()) {
      regionList.push_back(std::make_unique<Region>(
          std::set{std::make_pair(x, y)}, unsigned(1), unsigned(4)));
      return;
    }

    // This jining a single region
    if (it1 == it2) {
      (*it1)->area += 1;
      (*it1)->plants.insert({x, y});
      return;
    }

    // Thius is brigging 2 distinct
    if (it1 != regionList.end() && it2 != regionList.end()) {
      (*it1)->area += (*it2)->area + 1;
      (*it1)->perimeter += (*it2)->perimeter;
      (*it1)->plants.insert((*it2)->plants.begin(), (*it2)->plants.end());
      (*it1)->plants.insert({x, y});
      regionList.erase(it2);
      return;
    }

    if (it1 == regionList.end()) {
      std::swap(it1, it2);
    }

    // here we appending to it1
    (*it1)->area += 1;
    (*it1)->perimeter += 2;
    (*it1)->plants.insert({x, y});
  }

  auto price() {
    unsigned int result = 0;
    for (const auto &[_, regionList] : regions) {
      for (const auto &region : regionList) {
        result += region->area * region->perimeter;
      }
    }
    return result;
  }
};

auto calculate_1(std::string_view input) {
  RegionMap garden;
  for (auto [y, line] : getLines(input) | std::views::enumerate) {
    for (auto [x, chr] : line | std::views::enumerate) {
      garden.add(chr, x, y);
    }
  }

  return garden.price();
}

auto calculate_2(std::string_view input) {
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
