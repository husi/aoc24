#include <algorithm>
#include <iostream>
#include <map>
#include <ranges>
#include <regex>
#include <set>
#include <sstream>
#include <utility>
#include <vector>

#include "utils.hpp"

auto parseInput(std::string_view input) {
  auto lines = getLines(input);
  bool parse_rules = true;

  std::map<int, std::set<int>> rules;
  std::vector<std::vector<int>> updates;
  for (auto line : lines) {
    if ("" == line) {
      parse_rules = false;
      continue;
    }

    if (parse_rules) {
      auto index = line.find('|');
      rules[atoi(line.substr(0, index).c_str())].insert(
          atoi(line.substr(index + 1).c_str()));
    } else {
      std::vector<int> update;

      auto index = -1;
      do {
        auto start = index + 1;
        index = line.find(',', start);
        if (index == std::string::npos) {
          index = line.size();
        }
        update.push_back(atoi(line.substr(start, index - start).c_str()));

      } while (index != line.size());
      updates.emplace_back(update);
    }
  }

  return std::make_pair(rules, updates);
}

int calculate_1(std::string_view input) {
  int result = 0;
  auto [rules, updates] = parseInput(input);

  for (auto update : updates) {
    bool update_is_good = true;
    for (int i = 0; i < update.size(); ++i) {
      for (int j = i + 1; j < update.size(); j++) {
        if (!rules[update[i]].contains(update[j])) {
          update_is_good = false;
          break;
        }
      }
      if (!update_is_good) {
        break;
      }
    }
    if (update_is_good) {
      result += update[update.size() / 2];
    }
  }

  return result;
}

auto fixUpdate(std::vector<int> &update, std::map<int, std::set<int>> rules) {
  // classic bubblesort is simple
  bool was_swap = true;
  while (was_swap) {
    was_swap = false;
    for (int i = 0; i < update.size() - 1; ++i) {
      if (rules[update[i + 1]].contains(update[i])) {
        std::swap(update[i], update[i + 1]);
        was_swap = true;
      }
    }
  }
}

int calculate_2(std::string_view input) {
  int result = 0;
  auto [rules, updates] = parseInput(input);

  for (auto update : updates) {
    bool update_is_good = true;
    for (int i = 0; i < update.size(); ++i) {
      for (int j = i + 1; j < update.size(); j++) {
        if (!rules[update[i]].contains(update[j])) {
          update_is_good = false;
          break;
        }
      }
      if (!update_is_good) {
        break;
      }
    }
    if (!update_is_good) {
      fixUpdate(update, rules);
      result += update[update.size() / 2];
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
