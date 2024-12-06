#include <algorithm>
#include <iostream>
#include <map>
#include <ranges>
#include <regex>
#include <sstream>
#include <utility>
#include <vector>

#include "utils.hpp"

auto rotate90(std::vector<std::string> lines) {
  auto l = lines[0].size();
  std::vector<std::string> result{l};
  for (auto line : lines) {
    for (auto i = 0; i < l; ++i) {
      result[l - i - 1].push_back(line[i]);
    }
  }

  return result;
}

// we know the input is squere
auto rotate45(std::vector<std::string> lines) {
  auto l = lines.size();

  std::vector<std::string> result;
  result.reserve(l * 2 - 1);

  for (size_t i = 0; i < l; ++i) {
    std::string l1, l2;
    l1.reserve(i + 1);
    l2.reserve(i + 1);

    for (size_t j = 0; j <= i; ++j) {
      l1.push_back(lines[i - j][j]);
      l2.push_back(lines[l - j - 1][l - i + j - 1]);
    }

    result.push_back(l1);
    if (i < l - 1) {
      result.push_back(l2);
    }
  }
  return result;
}

void printLines(std::vector<std::string> lines) {
  for (auto line : lines) {
    std::cout << line << std::endl;
  }
  std::cout << std::endl;
}

int calculate_1(std::string_view input) {
  int result = 0;
  auto lines = getLines(input);
  // std::cout << std::endl;

  std::vector<std::string> all_lines;

  all_lines.reserve(lines.size() * 6);

  auto rlines = rotate90(lines);
  auto lines45 = rotate45(lines);
  auto rlines45 = rotate45(rlines);

  std::regex xmas{
      R"-((?=(XMAS|SAMX)).)-"};  // to match overlaps use a positive lookahead

  for (auto map : {lines, rlines, lines45, rlines45}) {
    for (auto line : map) {
      std::regex_iterator it{line.begin(), line.end(), xmas};
      decltype(it) end{};
      for (; it != end; ++it) {
        result++;
      }
      // std::cout << line << ":" << result << std::endl;
    }
  }

  return result;
}

struct Window {
  Window(std::vector<std::string> lines, size_t x, size_t y) {
    _content.reserve(9);
    _content.append(lines[x].substr(y, 3));
    _content.append(lines[x + 1].substr(y, 3));
    _content.append(lines[x + 2].substr(y, 3));
  }

  bool match() { return std::regex_match(_content, _xmas); }
  std::string str() {
    std::stringstream stream;
    stream << _content.substr(0, 3) << std::endl
           << _content.substr(3, 3) << std::endl
           << _content.substr(6, 3) << std::endl;
    auto result = stream.str();
    return result;
  }
  std::string _content;
  std::regex _xmas{R"-(M.M.A.S.S|M.S.A.M.S|S.S.A.M.M|S.M.A.S.M)-"};
};

int calculate_2(std::string_view input) {
  int result = 0;
  auto lines = getLines(input);

  for (size_t y = 0; y < lines.size() - 2; ++y) {
    for (size_t x = 0; x < lines[y].size() - 2; ++x) {
      Window w{lines, x, y};
      // std::cout << w.str() << std::endl;
      if (w.match()) {
        result++;
      }
      // std::cout << result << std::endl;
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
