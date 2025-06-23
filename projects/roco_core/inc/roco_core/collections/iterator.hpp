#pragma once

#include "roco_core/roco_core.hpp"
#include <concepts>
namespace roco {
namespace core {
namespace collections {

template <typename T>
concept is_iterator = is_movable<T> && is_copyable<T> && requires(T t1, T t2) {
  typename T::t_elem;
  typename T::t_coll;
  typename T::t_ptr;
  typename T::t_ref;

  { t1.inc() } -> std::convertible_to<void>;
  { t1.get() } -> std::convertible_to<typename T::t_ref>;
  { t1.operator*() } -> std::convertible_to<typename T::t_ref>;

  { t1.ptr() } -> std::convertible_to<typename T::t_ptr>;
  { t1.operator->() } -> std::convertible_to<typename T::t_ptr>;
  { t1 == t2 } -> std::convertible_to<bool>;

  { t1.distance(t2) } -> std::convertible_to<size_t>;
};

template <typename T>
concept is_iterator_back = is_iterator<T> && requires(T t) {
  { t.dec() } -> std::same_as<void>;
  { t.distance_signed(t) } -> std::convertible_to<roco::core::ssize_t>;
};

template <typename T>
concept is_iterator_randacc =
    is_iterator<T> && is_iterator_back<T> && requires(T t, std::size_t count) {
      { t.inc(count) } -> std::same_as<void>;
      { t.dec(count) } -> std::same_as<void>;
    };

template <typename T> class span {
public:
  static_assert(is_iterator<T>);

public:
  span(T beg, T end) : b(beg), e(end) {}
  ~span() = default;

  span(const span &other) = default;
  span &operator=(const span &other) = default;

  span(span &&other) = default;
  span &operator=(span &&other) = default;

  T b;
  T e;
};

} // namespace collections
} // namespace core
} // namespace roco
