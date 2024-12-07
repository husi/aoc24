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

struct Bigint {
  std::vector<unsigned> digits;
  static const auto BASE = 1000U;
  Bigint() { digits.resize(1); }

  Bigint& operator+=(unsigned long long other) {
    size_t index = 0;
    unsigned remainder = 0;

    while (other > 0) {
      if (index == digits.size()) {
        digits.resize(index + 1);
      }
      auto cdigit = digits[index] + other % BASE;
      digits[index++] = cdigit % BASE;
      other = other / BASE + cdigit / BASE;
    }

    return *this;
  }
};

std::ostream& operator<<(std::ostream& s, const Bigint& other) {
  for (auto digit : other.digits | std::ranges::views::reverse) {
    s << digit << std::setw(3) << std::setfill('0');
  }

  return s;
}

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

Bigint calculate_1(std::string_view input) {
  Bigint result;
  for (auto [target, values] : parse(input)) {
    if (solvable(target, values.size() - 1, values)) {
      result += target;
    }
  }

  return result;
}

Bigint calculate_2(std::string_view input) {
  Bigint result;
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
