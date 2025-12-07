#pragma once

#include <ranges>
#include <string>
#include <string_view>

struct Day4 {
  constexpr size_t id() const { return 4; }

  // check the if position [i,j] is occupied, then check how many surrounding
  // positions are occupied; if more than 4 - return true
  bool check(const auto &mat, size_t i, size_t j) {
    const size_t h = mat.size() - 1;
    const size_t w = mat[0].size() - 1;
    const bool b00 = i >= 1;
    const bool b01 = i < h;
    const bool b10 = j >= 1;
    const bool b11 = j < w;

    if (mat[i][j] != '@') {
      return false;
    };

    size_t cnt = 0;
    cnt += b00 ? mat[i - 1][j] != '@' : 1;
    cnt += b01 ? mat[i + 1][j] != '@' : 1;
    cnt += b00 && b10 ? mat[i - 1][j - 1] != '@' : 1;
    cnt += b00 && b11 ? mat[i - 1][j + 1] != '@' : 1;
    cnt += b01 && b10 ? mat[i + 1][j - 1] != '@' : 1;
    cnt += b01 && b11 ? mat[i + 1][j + 1] != '@' : 1;
    cnt += b10 ? mat[i][j - 1] != '@' : 1;
    cnt += b11 ? mat[i][j + 1] != '@' : 1;

    return cnt > 4;
  }

  size_t problem1(const std::string_view &input) {

    // straightforward; iterate over all, check surrounding grid, increment
    // return accordingly

    std::vector<std::string_view> matrix;
    std::ranges::copy(
        input | std::views::split('\n') |
            std::views::filter(std::not_fn(std::ranges::empty)) |
            std::views::transform([](const std::ranges::view auto &view) {
              return std::string_view{view.begin(), view.end()};
            }),
        std::back_inserter(matrix));

    size_t ret = 0;
    for (size_t i = 0; i < matrix.size(); ++i) {
      for (size_t j = 0; j < matrix[0].size(); ++j) {
        ret += check(matrix, i, j);
      }
    }

    return ret;
  }

  size_t problem2(const std::string_view &input) {

    // this time we will be modifying the matrix on every simulation step ->
    // will create a list of mutable strings

    std::vector<std::string> matrix;
    std::ranges::copy(
        input | std::views::split('\n') |
            std::views::filter(std::not_fn(std::ranges::empty)) |
            std::views::transform([](const std::ranges::view auto &view) {
              return std::string{view.begin(), view.end()};
            }),
        std::back_inserter(matrix));

    size_t ret = 0;
    size_t cnt;
    do {
      cnt = 0;
      for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[0].size(); ++j) {
          if (check(matrix, i, j)) {
            // mark counted
            matrix[i][j] = 'x';
            ++cnt;
          }
        }
      }
      ret += cnt;

      // repeat until no rolls removed
    } while (cnt);

    return ret;
  }
};
