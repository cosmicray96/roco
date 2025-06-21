#pragma once

#include "roco_core/allocators/allocator.hpp"
#include "roco_core/allocators/heap.hpp"
#include "roco_core/collections/collection.hpp"
#include "roco_core/collections/iterator.hpp"
#include "roco_core/error_enum.hpp"
#include "roco_core/roco_core.hpp"
#include "roco_core/smart_ptr.hpp"

#include <algorithm>
#include <cstddef>
#include <ostream>
#include <type_traits>
#include <utility>

namespace roco {
namespace core {
namespace collections {

template <typename t_elem, bool t_is_const> class array_it;

template <typename T, size_t N, typename A> class array {
public:
  static_assert(allocators::is_allocator<A>, "A is not an allocator");

private:
  array() = default;

public:
  ~array() { destroy(); }

  array(const array &other) = delete;
  array &operator=(const array &other) = delete;

  array(array &&other) { swap(*this, other); }
  array &operator=(array &&other) {
    if (m_data == other.m_data) {
      return *this;
    }
    destroy();
    swap(*this, other);
    return *this;
  }

public:
  T &operator[](size_t index) { return m_data[index]; }
  const T &operator[](size_t index) const { return m_data[index]; }

public:
  size_t capacity() const { return N; }

  void destroy() {
    if (m_data) {
      allocators::delloc_type_array<A, T>(m_data, N);
      m_data = nullptr;
    }
  }

public:
  array_it<T, false> to_array_it_beg() { return array_it<T, false>(m_data); }
  array_it<T, false> to_array_it_end() {
    return array_it<T, false>(m_data + N);
  }
  array_it<T, true> to_array_it_beg_const() const {
    return array_it<T, true>(m_data);
  }
  array_it<T, true> to_array_it_end_const() const {
    return array_it<T, true>(m_data + N);
  }

public:
  friend void swap(array &a, array &b) { std::swap(a.m_data, b.m_data); }

  friend std::ostream &operator<<(std::ostream &os, const array &arr)
    requires roco::core::is_printable<T>
  {
    if (N == 0) {
      os << "[ ]";
      return os;
    }
    if (N == 1) {
      os << "[ " << arr.m_data[0] << " ]";
      return os;
    }
    os << "[ ";
    for (size_t i = 0; i < N - 1; i++) {
      os << arr.m_data[i] << ", ";
    }
    os << arr.m_data[N - 1] << " ]";
    return os;
  }

public:
  template <typename... Args>
  static roco::core::result<array, roco::core::error_enum> make(Args &&...args)
    requires is_movable<T> &&
             (std::is_default_constructible_v<T> || sizeof...(Args) > 0)
  {
    array<T, N, A> arr;
    roco::core::result<T *, roco::core::error_enum> res =
        allocators::alloc_type_array<A, T>(N, std::forward<Args>(args)...);

    if (res.has_error()) {
      return {res.take_error()};
    }
    arr.m_data = res.take_value();
    return {std::move(arr)};
  }

private:
  T *m_data = nullptr;
};

///////

template <typename T, bool t_is_const> class array_it {
public:
  using t_elem = T;
  using t_ptr = std::conditional_t<t_is_const, const t_elem *, t_elem *>;
  using t_ref = std::conditional_t<t_is_const, const t_elem &, t_elem &>;

public:
  array_it(t_elem *ptr) : m_ptr(ptr) {}
  virtual ~array_it() = default;
  array_it(const array_it &other) = default;
  array_it &operator=(const array_it &other) = default;
  array_it(array_it &&other) = default;
  array_it &operator=(array_it &&other) = default;

public:
  void inc() { m_ptr++; }
  void dec() { m_ptr--; }
  void inc(size_t count) { m_ptr += count; }
  void dec(size_t count) { m_ptr -= count; }
  t_ref get() { return *m_ptr; }
  t_ptr ptr() { return m_ptr; }

public:
  t_ref operator*() { return *m_ptr; }
  t_ptr operator->() { return m_ptr; }
  bool operator==(const array_it &other) const { return other.m_ptr == m_ptr; }

public:
  friend void swap(array_it &a, array_it &b) { std::swap(a.m_ptr, b.m_ptr); }

private:
  t_ptr m_ptr;
};

///////////

} // namespace collections
} // namespace core
} // namespace roco
