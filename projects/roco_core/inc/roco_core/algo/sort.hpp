#pragma once

#include "roco_core/collections/iterator.hpp"
#include <concepts>
namespace roco {
namespace core {
namespace algo {

template <typename T>
concept is_sortable = requires(T t) {
  { t == t } -> std::convertible_to<bool>;
  { t > t } -> std::convertible_to<bool>;
  { t < t } -> std::convertible_to<bool>;
  {
    swap(t, t)
  } -> std::convertible_to<void>;
};

template <typename T>
  requires collections::is_iterator<T> &&
           is_sortable<typename T::t_elem>
void sort(T it1, T it2) {

  while (it1 != it2) {

    if (*it1 > *it2) {
      swap(*it1, *it2);
    }

    it1.inc();
  }
}

} // namespace algo

} // namespace core

} // namespace roco
