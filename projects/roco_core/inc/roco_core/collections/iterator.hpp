#pragma once

#include <concepts>
namespace roco {
namespace core {
namespace collections {

template <typename T>
concept is_iterator = requires(T t1, T t2) {
  typename T::t_elem;
  typename T::t_ptr;
  typename T::t_ref;

  { t1.inc() } -> std::convertible_to<void>;
  {
    t1.get()
  }
  -> std::convertible_to<typename T::t_ref>;
  {
    t1.operator*()
  }
  -> std::convertible_to<typename T::t_ref>;

  {
    t1.ptr()
  }
  -> std::convertible_to<typename T::t_ptr>;
  {
    t1.operator->()
  }
  -> std::convertible_to<typename T::t_ptr>;
  { t1 == t2 } -> std::convertible_to<bool>;
};

template <typename T>
concept is_iterator_back =
    is_iterator<T> && requires(T t) {
      { t.dec() } -> std::same_as<void>;
    };

template <typename T>
concept is_iterator_randacc =
    is_iterator<T> && is_iterator_back<T> &&
    requires(T t, std::size_t count) {
      { t.inc(count) } -> std::same_as<void>;
      { t.dec(count) } -> std::same_as<void>;
    };

} // namespace collections
} // namespace core
} // namespace roco
