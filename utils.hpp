#pragma once

#include <filesystem>
#include <fstream>
#include <optional>
#include <regex>
#include <string_view>
#include <strstream>

auto getLines(std::string_view input) {
  std::istringstream s(input.data());
  std::vector<std::string> result;
  do {
    std::string line;
    std::getline(s, line);
    result.push_back(line);
  } while (!s.eof());

  return result;
}

template <class T>
auto splitLine(std::string_view line) {
  std::istrstream stream{line.data()};
  std::vector<T> result;
  while (!stream.eof()) {
    T value;
    stream >> value;
    result.emplace_back(value);
  }

  return result;
}

void printLines(std::vector<std::string> lines) {
  for (auto line : lines) {
    std::cout << line << std::endl;
  }
  std::cout << std::endl;
}

std::optional<std::string> readInput(std::string_view file_name) {
  if (!std::filesystem::exists(file_name) ||
      !std::filesystem::is_regular_file(file_name)) {
    return {};
  }

  std::ifstream file_input(file_name.data());
  if (!file_input.is_open()) {
    return {};
  }

  return std::string{std::istreambuf_iterator{file_input}, {}};
}
