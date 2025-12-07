
#include "aoc.h"
#include "day_1.h"
#include "day_10.h"
#include "day_2.h"
#include "day_3.h"
#include "day_4.h"
#include "day_5.h"
#include "day_6.h"
#include "day_7.h"
#include "day_8.h"
#include "day_9.h"

#include <iostream>
#include <thread>
#include <unordered_set>

#include <ranges>

// append here
constexpr std::tuple<Day1, Day2, Day3, Day4, Day5, Day6, Day7, Day8, Day9,
                     Day10>
    puzzles;

int main(int argc, const char *argv[]) {
  using namespace std::string_view_literals;

  if (argc < 2) {
    std::cout << "you have to provide session id\n";
    return 0;
  }

  auto args = std::span{argv, argv + argc} | std::views::drop(1) |
              std::views::transform(
                  [](const auto &ptr) { return std::string_view{ptr}; });

  auto sessionId = args.front();
  // puzzles to solve (if empty - solve all)
  std::unordered_set<size_t> enabled;
  std::ranges::copy(args | std::views::drop(1) |
                        std::views::transform([](const auto &str) {
                          return std::strtoul(str.data(), nullptr, 10);
                        }),
                    std::inserter(enabled, enabled.end()));

  // solve both puzzles of one day
  auto evaluate = [&sessionId, &enabled](Day auto day) {
    if (enabled.contains(day.id()) || enabled.empty()) {
      auto input = AoC(day.id(), sessionId).fetchInput();
      std::cout << std::format("{:<10}{:<20}{:<20}\n"sv, day.id(),
                               day.problem1(input), day.problem2(input));
    }
  };

  // solve all days
  std::apply(
      [&](Day auto... day) {
        std::cout << std::format("{:<10}{:<20}{:<20}\n{:->{}}\n"sv, "Day"sv,
                                 "Puzzle 1"sv, "Puzzle 2"sv, ""sv, 50);

        // safe since jthread auto-joins
        std::make_tuple(std::jthread(evaluate, day)...);
      },
      puzzles);

  return 0;
}
