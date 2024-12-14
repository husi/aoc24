#include <algorithm>
#include <cassert>
#include <iostream>
#include <list>
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
  unsigned int length;

  uint64_t checksum() const {
    return id == -1 ? 0 : (index + index + length - 1) * length / 2 * id;
  }
};
using NodeList = std::list<Node>;

auto parse(std::string_view input) {
  unsigned id = 0;
  unsigned index = 0;
  auto pipeline =
      input | std::views::take(input.size() - 1) |
      std::views::transform([](auto c) -> unsigned int { return c - '0'; }) |
      std::views::enumerate | std::views::transform([&id, &index](auto value) {
        auto [i, v] = value;
        Node result(i % 2 ? -1 : id++, index, v);
        index += v;
        return result;
      });

  return NodeList(pipeline.begin(), pipeline.end());
}

NodeList::iterator distribute_last(NodeList &nodes, NodeList::iterator start) {
  auto last = nodes.back();
  nodes.pop_back();

  while (last.length > 0) {
    while (start->id != -1) {
      ++start;
      if (start == nodes.end()) {
        if (last.length > 0) {
          nodes.push_back(last);
        }
        return start;  // we are finished;
      }
    }

    auto &gap = *start;

    if (gap.length <= last.length) {
      gap.id = last.id;
      last.length -= gap.length;
    } else {
      auto remainder = gap.length - last.length;
      gap.id = last.id;
      gap.length = last.length;

      start =
          nodes.insert(++start, Node(-1, gap.index + last.length, remainder));
      last.length = 0;  // we are done
    }
  }

  while (nodes.back().id == -1) {
    nodes.pop_back();
  }

  return start;
}

auto checksum(NodeList nodes) {
  return std::ranges::fold_left_first(
             nodes | std::views::transform(
                         [](auto node) { return node.checksum(); }),
             std::plus<uint64_t>())
      .value();
}
auto calculate_1(std::string_view input) {
  auto nodes = parse(input);

  auto start = nodes.begin();

  while (start != nodes.end()) {
    start = distribute_last(nodes, start);
  }

  return checksum(nodes);
}

auto calculate_2(std::string_view input) {
  auto nodes = parse(input);

  auto end = nodes.end();
  --end;

  std::set<size_t> checked;

  while (end != nodes.begin()) {
    while (end->id == -1 || checked.contains(end->id)) {
      --end;
    }

    auto start = nodes.begin();
    if (end == start) {
      break;
    }

    checked.insert(end->id);

    while (start->id != -1 || start->length < end->length) {
      ++start;
      if (end == start) {
        break;
      }
    }

    if (start == end) {
      continue;
    }

    nodes.insert(start, *end)->index = start->index;
    start->length -= end->length;
    start->index += end->length;
    end->id = -1;
  }

  return checksum(nodes);
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
