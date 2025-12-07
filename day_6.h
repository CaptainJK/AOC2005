#pragma once

#include <algorithm>
#include <ranges>
#include <string_view>
#include <vector>

struct Day6 {

  constexpr size_t id() const { return 6; }

  // split lines -> remove empty (if any) -> create string_view
  auto inputToLines(const std::string_view &input) const {
    return input | std::views::split('\n') |
           std::views::filter(std::not_fn(std::ranges::empty)) |
           std::views::transform([](const std::ranges::view auto &view) {
             return std::string_view{view.begin(), view.end()};
           });
  };

  size_t problem1(const std::string_view &input) {
    // solution is straightforward, only catch is to not mess up boundaries

    // split lines
    std::vector<std::string_view> lines;
    std::ranges::copy(inputToLines(input), std::back_inserter(lines));

    // last line contains operations
    const auto &ops = lines.back();

    // basically, iterate over all columns;
    // when there is an operation at column j calculate result = reduce(row,
    // op); add the result to ret
    size_t ret = 0;
    for (size_t j = 0; j < ops.size(); ++j) {
      if (!std::isspace(ops[j])) {
        bool isOpAdd = ops[j] == '+';

        // result of the operation performed on all values from the column
        size_t result = isOpAdd ? 0 : 1;

        for (size_t i = 0; i < lines.size() - 1; ++i) {
          // substring of line[i] starting at j for i = 0..N-1 contains numbers
          auto num = std::strtoul(lines[i].substr(j).data(), nullptr, 10);
          result = isOpAdd ? result + num : result * num;
        }

        // grand total
        ret += result;
      }
    }
    return ret;
  }

  size_t problem2(const std::string_view &input) {

    // same approach - split lines, iterate over "operations" line until hit '+'
    // or '*'; after that - instead of converting numbers directly - convert
    // them in vertical slices

    std::vector<std::string_view> lines;
    std::ranges::copy(inputToLines(input), std::back_inserter(lines));

    // last line contains operations
    const auto &ops = lines.back();

    size_t ret = 0;
    for (size_t j = 0; j < ops.size(); ++j) {
      if (!std::isspace(ops[j])) {
        bool isOpAdd = ops[j] == '+';

        size_t result = isOpAdd ? 0 : 1;

        // start sweeping over numbers column by column from the position of
        // current operation
        size_t k = j;
        while (true) {

          // manual strtoul
          size_t num = 0;
          for (size_t i = 0; i < lines.size() - 1; ++i) {
            if (lines[i][k] != ' ') {
              num = num * 10 + (lines[i][k] - '0');
            }
          }

          // since there are no zeros in the input vector, num == 0 indicates
          // that no lines contained digit => separator column;
          // don't perform the operation in this case, as multiplication by 0
          // will break the result
          if (num == 0) {
            break;
          }

          result = isOpAdd ? result + num : result * num;

          // last column?
          if (++k >= lines.back().size()) {
            break;
          }
        }

        ret += result;
      }
    }

    return ret;
  }
};
