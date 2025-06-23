#pragma once
#include "roco_core/collections/iterator.hpp"

#include <concepts>
#include <utility>

namespace roco {
namespace core {
namespace algo {

namespace _hide {
using std::swap;
template <typename T>
concept has_adl_swap = requires(T t) {
  { swap(t, t) } -> std::convertible_to<void>;
};

} // namespace _hide

template <typename T>
concept is_sortable = _hide::has_adl_swap<T> && requires(T t) {
  { t == t } -> std::convertible_to<bool>;
  { t > t } -> std::convertible_to<bool>;
  { t < t } -> std::convertible_to<bool>;
};

template <typename T>
  requires collections::is_iterator<T> && is_sortable<typename T::t_elem>
void sort(T beg, T end) {
  using std::swap;
  T it1 = beg;
  T it2 = beg;
  it2.inc();
  if (it2 == end) {
    return;
  }

  while (it1 != end) {
    it2 = it1;
    it2.inc();
    while (it2 != end) {
      if (*it1 > *it2) {
        swap(*it1, *it2);
      }
      it2.inc();
    }
    it1.inc();
  }
}

template <typename T>
  requires collections::is_iterator<T> && is_sortable<typename T::t_elem>
void selection_sort(collections::span<T> s) {
  using std::swap;

  for (T it1 = s.b; it1 != s.e; it1.inc()) {
    T smallest = it1;
    for (T it2 = it1; it2 != s.e; it2.inc()) {
      if (*it2 < *smallest) {
        smallest = it2;
      }
    }
    if (!(smallest == it1)) {
      swap(*it1, *smallest);
    }
  }
}

template <typename T>
  requires collections::is_iterator<T> && is_sortable<typename T::t_elem>
void bubble_sort(collections::span<T> s) {
  using std::swap;

  for (T it1 = s.b; it1 != s.e; it1.inc()) {
    for (T it2 = it1; it2 != s.e; it2.inc()) {
      if (*it1 > *it2) {
        swap(*it1, *it2);
      }
    }
  }
}

} // namespace algo
} // namespace core
} // namespace roco
