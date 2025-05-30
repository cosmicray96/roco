#pragma once

#include <concepts>
#include <iostream>
#include <type_traits>

namespace roco_core {

template <typename T>
concept is_printable = requires(std::ostream &os, T a) {
    { os << a } -> std::same_as<std::ostream &>;
};

class roco_core {
  public:
    static int add(int a, int b);
};
} // namespace roco_core
