#pragma once

#include <concepts>
#include <cstdlib>
#include <iostream>

namespace roco_core {

template <typename T>
concept is_printable = requires(std::ostream &os, T a) {
    { os << a } -> std::same_as<std::ostream &>;
};

void crash_program(const char *msg);

class roco_core {
  public:
    static int add(int a, int b);
};
} // namespace roco_core
