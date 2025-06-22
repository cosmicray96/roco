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
};
/*
template <typename T>
  requires collections::is_iterator<T> &&
           is_sortable<typename T::t_elem>
T::t_coll sort(T it1, T it2) {

}
*/

} // namespace algo

} // namespace core

} // namespace roco
