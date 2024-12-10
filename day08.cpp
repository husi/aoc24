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

struct GridPoint {
  size_t x;
  size_t y;

  bool operator<(const GridPoint& other) const {
    return x < other.x ? true : x == other.x && y < other.y;
  }
};

struct GridMarker : public GridPoint {
  char symbol = '.';
};

class Grid {
 public:
  static Grid parse(const std::string_view source) {
    Grid result;
    result.grid = std::move(getLines(source));
    return result;
  }

  std::optional<GridMarker> marker(size_t x, size_t y) const {
    if (x < 0 || y < 0 || x >= grid[0].size() || y >= grid.size()) {
      return {};
    }

    return GridMarker{x, y, grid[y][x]};
  }

  auto getMarkers() {
    auto transformation =
        grid | std::views::enumerate |
        std::views::transform([](const auto& value) {
          auto& li = std::get<0>(value);
          auto& line = std::get<1>(value);
          auto markers = line | std::views::enumerate |
                         std::views::filter([](const auto& value) {
                           return '.' != std::get<1>(value);
                         }) |
                         std::views::transform([li](const auto& value) {
                           const auto& index = std::get<0>(value);
                           const auto& chr = std::get<1>(value);
                           return GridMarker{static_cast<size_t>(index),
                                             static_cast<size_t>(li), chr};
                         });
          return markers;
        }) |
        std::views::join;

    std::vector<GridMarker> result;
    for (auto marker : transformation) {
      result.emplace_back(marker);
    }

    return result;
  }

 private:
  std::vector<std::string> grid;

  friend std::ostream& operator<<(std::ostream& os, const Grid& grid) {
    for (auto line : grid.grid) {
      os << line << std::endl;
    }

    return os;
  }
};

auto get_antinodes(const Grid& grid, const std::set<GridMarker>& group) {
  std::set<GridMarker> antinodes;

  if (group.size() < 2) {
    return antinodes;
  }

  auto first = *group.begin();
  auto rest = std::set<GridMarker>(++group.begin(), group.end());

  for (auto marker : rest) {
    long diffx = first.x - marker.x;
    long diffy = first.y - marker.y;

    if (auto antinode = grid.marker(first.x + diffx, first.y + diffy)) {
      antinodes.insert(antinode.value());
    }
    if (auto antinode = grid.marker(marker.x - diffx, marker.y - diffy)) {
      antinodes.insert(antinode.value());
    }
  }

  auto new_antinodes = get_antinodes(grid, rest);

  antinodes.insert(new_antinodes.begin(), new_antinodes.end());

  return antinodes;
}

auto get_antinodes2(const Grid& grid, const std::set<GridMarker>& group) {
  std::set<GridMarker> antinodes;

  if (group.size() < 2) {
    return antinodes;
  }

  auto first = *group.begin();
  auto rest = std::set<GridMarker>(++group.begin(), group.end());

  for (auto marker : rest) {
    long diffx = first.x - marker.x;
    long diffy = first.y - marker.y;

    auto posx = first.x;
    auto posy = first.y;
    while (auto antinode = grid.marker(posx, posy)) {
      antinodes.insert(antinode.value());
      posx += diffx;
      posy += diffy;
    }
    posx = marker.x;
    posy = marker.y;
    while (auto antinode = grid.marker(posx, posy)) {
      antinodes.insert(antinode.value());
      posx -= diffx;
      posy -= diffy;
    }
  }

  auto new_antinodes = get_antinodes2(grid, rest);

  antinodes.insert(new_antinodes.begin(), new_antinodes.end());

  return antinodes;
}

auto calculate_1(std::string_view input) {
  auto grid = Grid::parse(input);

  auto markers = grid.getMarkers();

  std::map<char, std::set<GridMarker> > groups;

  for (auto marker : markers) {
    groups[marker.symbol].insert(marker);
  }

  std::set<GridMarker> antinodes;

  for (const auto& [chr, group] : groups) {
    auto new_antinodes = get_antinodes(grid, group);
    antinodes.insert(new_antinodes.begin(), new_antinodes.end());
  }

  return antinodes.size();
}

auto calculate_2(std::string_view input) {
  auto grid = Grid::parse(input);

  auto markers = grid.getMarkers();

  std::map<char, std::set<GridMarker> > groups;

  for (auto marker : markers) {
    groups[marker.symbol].insert(marker);
  }

  std::set<GridMarker> antinodes;

  for (const auto& [chr, group] : groups) {
    auto new_antinodes = get_antinodes2(grid, group);
    antinodes.insert(new_antinodes.begin(), new_antinodes.end());
  }

  return antinodes.size();
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
