#pragma once

#include <queue>
#include <ranges>

struct Day8 {
  constexpr size_t id() const { return 8; }

  // pair of indices in junction box coordinates array
  using IdxPair = std::pair<size_t, size_t>;

  // Disjoint set
  struct DSet {

    DSet(size_t s) : parents(s) {
      for (size_t i = 0; i < parents.size(); ++i) {
        // pointer to parent
        parents[i].first = i;
        // number of childern (including self)
        parents[i].second = 1;
      }
    }

    // find root
    auto find(size_t i) {
      while (parents[i].first != i) {
        i = parents[i].first;
      }
      return parents[i];
    }

    // join sets and return union size
    size_t join(size_t i, size_t j) {
      auto pi = find(i).first;
      auto pj = find(j).first;

      if (pi == pj) {
        return parents[pi].second;
      }

      // summ children sizes
      parents[pi].second += parents[pj].second;

      // dont' care about optimization
      parents[pj].first = pi;

      return parents[pi].second;
    }

    std::vector<std::pair<size_t, size_t>> parents;
  };

  // Eucledian distance
  static inline auto dist(const auto &p0, const auto &p1) {
    const auto [x0, y0, z0] = p0;
    const auto [x1, y1, z1] = p1;
    const auto dx = x1 - x0;
    const auto dy = y1 - y0;
    const auto dz = z1 - z0;
    return dx * dx + dy * dy + dz * dz;
  }

  auto inputToPointList(const std::string_view &input) const {
    std::vector<std::tuple<long, long, long>> ret;
    std::ranges::copy(
        input | std::views::split('\n') |
            std::views::filter(std::not_fn(std::ranges::empty)) |
            std::views::transform([](const std::ranges::view auto &view) {
              char *next;
              auto x = std::strtol(view.data(), &next, 10);
              auto y = std::strtol(next + 1, &next, 10);
              auto z = std::strtol(next + 1, nullptr, 10);
              return std::make_tuple(x, y, z);
            }),
        std::back_inserter(ret));
    return ret;
  }

  constexpr auto comparator(const auto &points) const {
    return [&points](const IdxPair &p0, const IdxPair &p1) {
      return dist(points[p0.first], points[p0.second]) >
             dist(points[p1.first], points[p1.second]);
    };
  }

  size_t problem1(const std::string_view &input) {

    // connect closest nearest boxes, sort by distance between pairs
    // then pick 1000 closest pairs and join them
    // assuming the result must yield at least 3 disjointed sets - sort them by
    // size, pick top 3 and multiply

    auto points = inputToPointList(input);

    std::priority_queue<IdxPair, std::vector<IdxPair>,
                        decltype(comparator(points))>
        sorted{comparator(points)};

    // sort pairs by relative distance
    for (size_t i = 0; i < points.size(); ++i) {
      for (size_t j = i + 1; j < points.size(); ++j) {
        sorted.emplace(i, j);
      }
    }

    // join pairs
    DSet ds{points.size()};
    for (size_t c = 0; c < 1000; ++c) {
      auto [i, j] = sorted.top();
      sorted.pop();
      ds.join(i, j);
    }

    // sort subsets by their size (only store sizes)
    std::priority_queue<size_t> sizes;
    for (size_t i = 0; i < points.size(); ++i) {

      // only for root nodes!
      if (ds.parents[i].first == i) {
        sizes.push(ds.parents[i].second);
      }
    }

    // multiply 3 top sizes
    size_t ret = 1;
    for (size_t i = 0; i < 3; ++i) {
      ret *= sizes.top();
      sizes.pop();
    }

    return ret;
  }

  size_t problem2(const std::string_view &input) {

    // same approach, but join all nodes
    // once the joined subset size becomes equal to total number of nodes - pick
    // 2 lately joined boxes coordinates and return the multiplication

    auto points = inputToPointList(input);

    std::priority_queue<IdxPair, std::vector<IdxPair>,
                        decltype(comparator(points))>
        sorted{comparator(points)};

    for (size_t i = 0; i < points.size(); ++i) {
      for (size_t j = i + 1; j < points.size(); ++j) {
        sorted.emplace(i, j);
      }
    }

    DSet ds{points.size()};
    size_t ret;
    while (!sorted.empty()) {
      auto [i, j] = sorted.top();
      sorted.pop();

      if (ds.join(i, j) == points.size()) {
        // ds represents one single set; done
        ret = std::get<0>(points[i]) * std::get<0>(points[j]);
        break;
      }
    }

    return ret;
  }
};
