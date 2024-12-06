#pragma once

#include <filesystem>
#include <fstream>
#include <optional>
#include <regex>
#include <string_view>
#include <strstream>

auto getLines(std::string_view input) {
  static const std::regex line{R"-(([^\n]+)\n?)-"};
  auto it = std::regex_iterator<std::string_view::iterator>(input.begin(),
                                                            input.end(), line);
  std::vector<std::string> result;
  for (decltype(it) last; it != last; ++it) {
    result.push_back((*it).str(1));
  }
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
