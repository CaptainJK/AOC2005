#pragma once

#include <algorithm>
#include <ranges>

struct Day9 {

  constexpr size_t id() const { return 9; }

  struct Point {
    long x;
    long y;
  };

  using Polygon = std::vector<Point>;

  // parse input to vector of 2d points (polygon)
  auto inputToPolygon(const std::string_view &input) {
    Polygon ret;
    std::ranges::copy(
        input | std::views::split('\n') |
            std::views::filter(std::not_fn(std::ranges::empty)) |
            std::views::transform([](const std::ranges::view auto &view) {
              char *next;
              auto x = std::strtol(view.data(), &next, 10);
              auto y = std::strtol(next + 1, nullptr, 10);
              return Point{x, y};
            }),
        std::back_inserter(ret));

    return ret;
  }

  inline size_t area(const Point &p0, const Point &p1) const {
    // tiles have size :)
    return (std::abs(p1.x - p0.x) + 1) * (std::abs(p1.y - p0.y) + 1);
  }

  bool pointInPolygon(const Polygon &poly, const Point &p) {
    size_t cnt = 0;
    bool off = poly[0].y == poly[1].y;
    for (size_t i = 0; i < poly.size() - 1; i += 2) {
      const auto &n0 = poly[i + off];
      const auto &n1 = poly[(i + 1 + off) % poly.size()];

      auto [nt, nb] = std::minmax(n0.y, n1.y);

      bool yIntersect = p.y >= nt && p.y < nb;
      if (n0.x == p.x && yIntersect) {
        return true;
      }

      cnt += n0.x > p.x && yIntersect;
    }
    return cnt % 2;
  }

  bool rectOverlapsPolygon(const Polygon &poly, const Point &p0,
                           const Point &p1) {
    auto [l, r] = std::minmax(p0.x, p1.x);
    auto [t, b] = std::minmax(p0.y, p1.y);

    for (size_t i = 0; i < poly.size(); ++i) {
      const auto &n0 = poly[i];
      const auto &n1 = poly[(i + 1) % poly.size()];

      auto [nl, nr] = std::minmax(n0.x, n1.x);
      auto [nt, nb] = std::minmax(n0.y, n1.y);

      if (!(nl >= r || nr <= l || nt >= b || nb <= t)) {
        return true;
      }
    }

    return false;
  }

  size_t problem1(const std::string_view &input) {
    const auto poly = inputToPolygon(input);

    // maybe there is some clever algorithm to do it in one pass, but I'm too
    // lazy, so here is O(n^2) and it runs in a reasonable time

    size_t ret = 0;

    for (size_t i = 0; i < poly.size(); ++i) {
      for (size_t j = i + 1; j < poly.size(); ++j) {
        ret = std::max(ret, area(poly[i], poly[j]));
      }
    }

    return ret;
  }

  size_t problem2(const std::string_view &input) {

    // same; but only count area if the rectangle fits in the polygon;
    // for that it must meet following conditions:
    // - opposite corners not belonging to polygon points must also be within
    // polygon
    // - no polygon points must be found wighin the rectangle
    // - rectangle sides must not intersect any of polygon segments

    const auto poly = inputToPolygon(input);

    long ret = 0;
    for (size_t i = 0; i < poly.size(); ++i) {
      for (size_t j = i + 1; j < poly.size(); ++j) {
        const auto &p0 = poly[i];
        const auto &p1 = poly[j];
        auto s = area(p0, p1);

        // cheap checks go first
        if (ret < s && pointInPolygon(poly, Point{p1.x, p0.y}) &&
            pointInPolygon(poly, Point{p0.x, p1.y}) &&
            !rectOverlapsPolygon(poly, p0, p1)) {
          ret = s;
        }
      }
    }
    return ret;
  }
};
