#pragma once

#include <numeric>
#include <ranges>

struct Day3 {

  constexpr size_t id() const { return 3; }

  auto maxSubsequenceOfLengthK(const std::string_view &line, size_t k) const {

    // strategy - maximize most significant digit; if line.size() == N, then it
    // must be found in range [0..N-k-1]

    size_t ret = 0;
    auto it = line.begin();
    for (int i = k - 1; i >= 0; --i) {
      it = std::max_element(it, std::prev(line.end(), i));
      ret = ret * 10 + (*it - '0');
      ++it;
    }
    return ret;
  }

  // both problems solved similarly
  auto solve(const std::string_view &input, size_t k) const {
    auto ret =
        input | std::views::split('\n') |
        std::views::filter(std::not_fn(std::ranges::empty)) |
        std::views::transform([this, k](const std::ranges::view auto &view) {
          return maxSubsequenceOfLengthK(
              std::string_view{view.begin(), view.end()}, k);
        });
    return std::accumulate(ret.begin(), ret.end(), 0ull);
  }

  size_t problem1(const std::string_view &input) { return solve(input, 2); }

  size_t problem2(const std::string_view &input) { return solve(input, 12); }
};
