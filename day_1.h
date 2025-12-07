#pragma once

#include <ranges>

struct Day1 {
  constexpr size_t id() const { return 1; }

  auto inputToInstructionList(const std::string_view &input) const {
    return input | std::views::split('\n') |
           std::views::filter(std::not_fn(std::ranges::empty)) |
           std::views::transform([](const std::ranges::view auto &view) {
             return (view[0] == 'L' ? -1 : 1) *
                    std::strtol(view.data() + 1, nullptr, 10);
           });
  };

  // c++ modulo operator is not true mathematical modulo as it returns signed
  // value with sign of divident
  auto trueMod(std::signed_integral auto val, std::unsigned_integral auto k) {
    return (k + val % k) % k;
  }

  size_t problem1(const std::string_view &input) {
    long long pos = 50;

    size_t ret = 0;

    for (auto ang : inputToInstructionList(input)) {
      pos = trueMod(pos + ang, 100u);
      ret += !pos;
    }

    return ret;
  }

  size_t problem2(const std::string_view &input) {
    long long pos = 50;

    size_t ret = 0;

    for (auto ang : inputToInstructionList(input)) {
      bool zeroCross = (pos > 0) && (pos + ang <= 0);
      pos += ang;
      ret += zeroCross + std::abs(pos) / 100;
      pos = trueMod(pos, 100u);
    }

    return ret;
  }
};
