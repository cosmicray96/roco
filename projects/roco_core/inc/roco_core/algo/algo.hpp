#pragma once

#include "roco_core/algo/sort.hpp"
#include "roco_core/collections/iterator.hpp"
#include <random>

namespace roco {
namespace core {
namespace algo {

template <typename T>
  requires collections::is_iterator_randacc<T> &&
           is_sortable<typename T::t_elem>
void randomize(T beg, T end) {}

} // namespace algo
} // namespace core
} // namespace roco
