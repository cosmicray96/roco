#pragma once

#include "roco_core/roco_core.hpp"

#include <algorithm>
#include <variant>

namespace roco {
namespace core {

template <typename T, typename E> class result {
public:
  static_assert(is_movable<T>, "T is not movable");
  static_assert(is_movable<E>, "E is not movable");

public:
  result(T &&t) : m_data(std::move(t)) {}
  result(E &&e) : m_data(std::move(e)) {}

  result(const result &other) = delete;
  result &operator=(const result &other) = delete;

  result(result &&other) = default;
  result &operator=(result &&other) = default;

public:
  bool has_error() const { return std::holds_alternative<E>(m_data); }

  E take_error() { return std::move(std::get<E>(m_data)); }
  T take_value() { return std::move(std::get<T>(m_data)); }

public:
  friend void swap(result &a, result &b) {
    using std::swap;
    swap(a.m_data, b.m_data);
  }

private:
  std::variant<T, E> m_data;
};

////////

template <typename T> class optional {
public:
  static_assert(is_movable<T>, "T is not.movable");

public:
  optional(T &&data) : m_data(std::move(data)) {}
  optional() {}

  optional(const optional &other) = delete;
  optional &operator=(const optional &other) = delete;

  optional(optional &&other) = default;
  optional &operator=(optional &&other) = default;

public:
  bool has_value() const { return std::holds_alternative<T>(m_data); }

  T take_value() { return std::move(std::get<T>(m_data)); }

public:
  friend void swap(optional &a, optional &b) {
    using std::swap;
    swap(a.m_data, b.m_data);
  }

private:
  std::variant<std::monostate, T> m_data;
};

} // namespace core
} // namespace roco
