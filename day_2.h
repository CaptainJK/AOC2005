#pragma once

#include "range_converter.h"
#include <ranges>

struct Day2 {
  constexpr size_t id() const { return 2; }

  // split words (comma-sep), transform to pairs [from,to]
  auto inputToRanges(const std::string_view &input) const {
    return input | std::views::split(',') |
           std::views::transform(RangeConverter{});
  };

  size_t problem1(const std::string_view &input) {

    // couldn't come up with any better solution; simply iterate over all values
    // from the range and check if they are invalid; accumulate invalid values

    size_t ret = 0;
    auto accumulateInvalidValues = [&ret](std::unsigned_integral auto val) {
      // find number of digits; if it is even, then divide by pow(10, x), where
      // x is half of vals' digits count; the division result must be equal to
      // mod(10, x) for the value to be 'invalid'

      size_t l = std::log10(val) + 1;
      if (!(l % 2)) {
        size_t p2 = std::pow(10, l / 2);
        if (val / p2 == val % p2) {
          ret += val;
        }
      }
    };

    for (const auto &range : inputToRanges(input)) {
      std::ranges::for_each(std::views::iota(range.first, range.second),
                            accumulateInvalidValues);
    }

    return ret;
  }

  size_t problem2(const std::string_view &input) {
    // same approach as in problem1

    size_t ret = 0;
    auto accumulateInvalidValues = [&](std::unsigned_integral auto val) {
      // roll val up to half of its digits count times; if at some point val is
      // equal to the original value, then this numbe is considered 'invalid'

      if (val >= 10) {
        size_t l = std::log10(val);
        size_t p = std::pow(10, l);

        auto v = val;
        for (size_t i = 0; i < l / 2 + 1; ++i) {
          v = (v % 10) * p + v / 10;
          if (v == val) {
            ret += val;
            return;
          }
        }
      }
    };

    for (const auto &range : inputToRanges(input)) {
      std::ranges::for_each(std::views::iota(range.first, range.second),
                            accumulateInvalidValues);
    }

    return ret;
  }
};
