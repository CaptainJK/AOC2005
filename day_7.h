#pragma once

#include <numeric>
#include <ranges>

struct Day7 {

  constexpr size_t id() const { return 7; }

  // split lines -> remove empty (if any) -> create string_view
  auto inputToLines(const std::string_view &input) const {
    return input | std::views::split('\n') |
           std::views::filter(std::not_fn(std::ranges::empty)) |
           std::views::transform([](const std::ranges::view auto &view) {
             return std::string_view{view.begin(), view.end()};
           });
  };

  size_t problem1(const std::string_view &input) {

    // we can consider system state at time point t; for that we will maintain
    // cross-section (beams) at every simulation step with with the cross
    // section it at hand, we can iterate over all splitters and see if there is
    // an incendent beam; count events of a beam meeting a splitter

    auto lines = inputToLines(input);

    auto it = lines.begin();

    // hold a set of all beams every step
    std::vector<int> beams((*it).size(), 0);
    beams[(*it).find('S')] = true;

    size_t ret = 0;
    for (it = std::next(it); it != lines.end(); ++it) {

      const auto &line = *it;

      // iterate over splitters
      size_t splitter = line.find('^');
      while (splitter != std::string_view::npos) {

        // if beam meets splitter - count the event and modify beams for the
        // next simulation step
        if (beams[splitter]) {
          ++ret;
          beams[splitter] = false;

          // apparently, there are no splitters on manifold boundaries (at least
          // not in my input), so it is safe to access +/-1 positions
          beams[splitter + 1] = true;
          beams[splitter - 1] = true;
        }
        splitter = line.find('^', splitter + 1);
      }
    }

    return ret;
  }

  size_t problem2(const std::string_view &input) {

    // with enough horsepower we can use bruteforce backtracking :)
    // but since we only want to count all possible paths, it is not necessary
    // the number of ways a beam reaches positions j at step i only depends on
    // the number of ways it reached positions at step i-1 and the confiuration
    // of splitters at step i; the final result is a summ of counts of possible
    // ways to reach position j at final simulation step

    auto lines = inputToLines(input);

    auto it = lines.begin();
    std::vector<size_t> count((*it).size(), 0);

    // only one way to reach this position (since the beam is already here)
    count[(*it).find('S')] = 1;

    for (it = std::next(it); it != lines.end(); ++it) {
      const auto &line = *it;
      for (size_t i = 0; i < line.size(); ++i) {
        if (line[i] == '^') {
          // if there is a splitter at i, then it 'diverts' count at i to the
          // left and to the right; ofcourse these diverted counts have to be
          // added
          count[i - 1] += count[i];
          count[i + 1] += count[i];
          count[i] = 0;
        }
      }
    }

    return std::accumulate(count.begin(), count.end(), 0ull);
  }
};
