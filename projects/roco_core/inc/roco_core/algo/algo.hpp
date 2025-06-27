#pragma once

#include "roco_core/algo/sort.hpp"
#include "roco_core/roco_core.hpp"

namespace roco {
namespace core {
namespace algo {

template <typename T, typename U>
  requires is_copyable<T> && is_sortable<T> && is_copyable<U> && is_sortable<U>
T min(const T &a, const U &b) {
  if (a < b) {
    return a;
  }
  return b;
}
template <typename T, typename U>
  requires is_copyable<T> && is_sortable<T> && is_copyable<U> && is_sortable<U>
T max(const T &a, const U &b) {
  if (a > b) {
    return a;
  }
  return b;
}

template <typename T> T abs(const T &t) {
  if (t > 0 || t == 0) {
    return t;
  }
  return -t;
}

} // namespace algo
} // namespace core
} // namespace roco
