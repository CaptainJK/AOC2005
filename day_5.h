#pragma once

#include "range_converter.h"
#include <algorithm>
#include <map>
#include <ranges>

struct Day5 {

  constexpr size_t id() const { return 5; }

  size_t problem1(const std::string_view &input) {
    using namespace std::string_view_literals;

    constexpr const auto wordDelim = '\n';
    // no sanity checks; assuming a valid input!
    const auto delimPos = input.find("\n\n"sv);

    // store ranges into a binary tree in form {start: end}
    std::map<size_t, size_t> m;
    auto insertRangeIntoMap = [&m](const auto &range) {
      const auto [it, ok] = m.try_emplace(range.first, range.second);
      if (!ok) {
        // if starting points of a range overlap - take the longest range
        it->second = std::max(it->second, range.second);
      }
    };

    // extract ranges
    std::ranges::for_each(input | std::views::take(delimPos) |
                              std::views::split(wordDelim) |
                              std::views::transform(RangeConverter{}),
                          insertRangeIntoMap);

    // extract ids
    auto ids = input | std::views::drop(delimPos + 2) |
               std::views::split(wordDelim) |
               std::views::transform([](const auto &s) {
                 return std::strtoul(s.data(), nullptr, 10);
               });

    // for each id:
    // - locate a range with left boundary less or equal than the id
    // - check if the id falls into the range and leave early if it does
    // - continue with the next range
    size_t ret = 0;
    for (const auto id : ids) {
      for (auto it = m.begin(); it != m.upper_bound(id); ++it) {
        if (id <= it->second) {
          ++ret;
          break;
        }
      }
    }

    return ret;
  }

  size_t problem2(const std::string_view &input) {

    // assume range = sum(f(id)), where f is a pulse function (1 - for ids
    // inside of the range and -1 - otherwise); we can accumulate derivative
    // over all ranges, and then integrate the result; accumulated function
    // 0-crossings mark transition points on 'id' axis

    using namespace std::string_view_literals;

    std::map<size_t, size_t> m;
    const auto insertRangeIntoMap = [&m](const auto &range) {
      auto [from, to] = range;
      // differentiating
      ++m[from];
      --m[to];
    };

    constexpr const auto wordDelim = '\n';
    // no sanity checks; assuming a valid input!
    const auto delimPos = input.find("\n\n"sv);
    std::ranges::for_each(input | std::views::take(delimPos) |
                              std::views::split(wordDelim) |
                              std::views::transform(RangeConverter{}),
                          insertRangeIntoMap);

    auto it = m.begin();
    size_t start = it->first;

    // window stores cumulative sum of all edges until [it] and its' previous
    // value cases:
    // [0] [1]
    //  0   0 - single point
    //  1   0 - end of all ranges
    //  0   1 - start of a range
    //  1   1 - inside of a range (don't care)
    size_t win[2] = {[1] = it->second};

    size_t ret = 0;
    for (it = std::next(it); it != m.end(); ++it) {
      // slide the window
      win[0] = std::exchange(win[1], win[1] + it->second);

      if (!win[0] && !win[1]) {
        // single point
        ++ret;
      } else if (!win[0] && win[1]) {
        // start of a new [non single-point] range
        start = it->first;
      } else if (win[0] && !win[1]) {
        // end of the [non single-point] range
        ret += it->first - start + 1;
      }
    }

    return ret;
  }
};
