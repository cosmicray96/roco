#pragma once

#include <concepts>
#include <cstdlib>
#include <iostream>
#include <type_traits>

namespace roco {
namespace core {

inline bool is_non_zero_pow_2(size_t x) { return x != 0 && (x & (x - 1)) == 0; }

template <typename T>
concept is_movable = requires(T a, T b) {
  { T(std::move(a)) };  // Move constructor
  { b = std::move(a) }; // Move assignment
};

template <typename T>

concept is_printable = requires(std::ostream &os, T a) {
  { os << a } -> std::same_as<std::ostream &>;
};

void crash_program(const char *msg);

class roco_core {
public:
  static int add(int a, int b);
};
} // namespace core
} // namespace roco
