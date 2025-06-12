#pragma once

#include <concepts>
#include <cstdlib>
#include <iostream>

namespace roco_core {

template <typename T>
concept is_printable = requires(std::ostream &os, T a) {
    { os << a } -> std::same_as<std::ostream &>;
};

template <typename T>
concept is_swappable = requires(T t1, T t2) { std::swap(t1, t2); } || requires(T t1, T t2) {
    T::swap(t1, t2);
} || requires(T t1, T t2) { swap(t1, t2); };
;

void crash_program(const char *msg);

class roco_core {
  public:
    static int add(int a, int b);
};
} // namespace roco_core
