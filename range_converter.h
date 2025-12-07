#pragma once

#include <stdlib.h>
#include <utility>

struct RangeConverter {

  // Helper class to convert range string in form '{from}-{to}' into a pair of
  // unsigned integers

  auto operator()(const auto &s) {
    char *next;
    size_t from = std::strtoul(s.data(), &next, 10);
    size_t to = std::strtoul(next + 1, nullptr, 10);
    return std::make_pair(from, to);
  };
};
