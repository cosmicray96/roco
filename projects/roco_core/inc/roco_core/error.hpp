#pragma once

#include "roco_core/roco_core.hpp"
#include <algorithm>
#include <concepts>
#include <variant>

namespace roco {
namespace core {

template <typename T, typename E>
    requires std::movable<T> && std::movable<E> && roco_core::is_swappable<T> &&
             roco_core::is_swappable<E>
class result {
  public:
    result(T t) : m_data(std::move(t)) {}
    result(E e) : m_data(std::move(e)) {}

    result(const result &other) = delete;
    result &operator=(const result &other) = delete;

    result(result &&other) : m_data(std::move(other.m_data)) {}
    result &operator=(result &&other) {
        swap(*this, other);
        return *this;
    }

  public:
    bool has_error() const { return std::holds_alternative<E>(m_data); }

    E take_error() { return std::move(std::get<E>(m_data)); }
    T take_value() { return std::move(std::get<T>(m_data)); }

  public:
    static void swap(result &a, result &b);

  private:
    std::variant<T, E> m_data;
};

template <typename T, typename E>
    requires std::movable<T> && std::movable<E> && roco_core::is_swappable<T> &&
             roco_core::is_swappable<E>
void result<T, E>::swap(result<T, E> &a, result<T, E> &b) {
    using std::swap;
    swap(a.m_data, b.m_data);
}

////////

template <typename T>
    requires std::movable<T> && roco_core::is_swappable<T>
class optional {

  public:
    optional(T data) : m_data(std::move(data)) {}
    optional() {}

    optional(const optional &other) = delete;
    optional &operator=(const optional &other) = delete;

    optional(optional &&other) : m_data(std::move(other.m_data)) {}
    optional &operator=(optional &&other) {
        m_data = std::move(other.m_data);
        return *this;
    }

  public:
    bool has_value() const { return std::holds_alternative<T>(m_data); }

    T take_value() { return std::move(std::get<T>(m_data)); }

  public:
    static void swap(optional &a, optional &b);

  private:
    std::variant<std::monostate, T> m_data;
};

template <typename T>
    requires std::movable<T> && roco_core::is_swappable<T>
void optional<T>::swap(optional<T> &a, optional<T> &b) {
    using std::swap;
    swap(a.m_data, b.m_data);
}
} // namespace core
} // namespace roco
