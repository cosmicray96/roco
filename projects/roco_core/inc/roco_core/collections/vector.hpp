#pragma once

#include "roco_core/algo/algo.hpp"
#include "roco_core/allocators/allocator.hpp"
#include "roco_core/collections/collection.hpp"
#include "roco_core/collections/iterator.hpp"
#include "roco_core/error.hpp"
#include "roco_core/roco_core.hpp"
#include "roco_core/smart_ptr.hpp"

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <ostream>
#include <utility>

namespace roco {
namespace core {
namespace collections {

class vector_d {
public:
  static size_t start_cap;
  static float growth_factor;
};

template <typename T, typename A> class vector {
public:
  using t_elem = T;

public:
  static_assert(is_collection_elem<T>);
  static_assert(allocators::is_allocator<A>);

private:
  vector() = default;

public:
  ~vector() { destory(); }

  vector(const vector &other) = delete;
  vector &operator=(const vector &other) = delete;

  vector(vector &&other) { swap(*this, other); }
  vector &operator=(vector &&other) {
    if (m_data == other.m_data) {
      return *this;
    }
    destory();
    vector temp(std::move(other));
    swap(*this, temp);
    return *this;
  }

public: // stdandard functions
  void push(T t) {
    if (m_count == m_capacity) {
      resize();
    }
    m_data[m_count] = std::move(t);
    m_count++;
  }

  size_t capacity() const { return m_capacity; }
  size_t count() const { return m_count; }

  void reserve(size_t n) {
    size_t space_left = m_capacity - m_count;
    if (space_left > n) {
      return;
    }
    resize(m_capacity + n);
  }

  void destory() {
    if (m_data) {
      allocators::delloc_type_array<A, T>(m_data, m_capacity);
      m_data = nullptr;
    }
  }

private:
  void resize() { resize(m_capacity * vector_d::growth_factor); }
  void resize(size_t new_cap) {
    auto res = allocators::alloc_type_array<A, T>(new_cap);
    if (res.has_error()) {
      crash_program(res.take_error(), "vector resize fail");
    }
    T *new_ptr = res.take_value();
    memcpy(new_ptr, m_data, m_capacity * sizeof(T));
    allocators::delloc_type_array<A>(m_data, m_capacity);
    m_data = new_ptr;
    m_capacity = new_cap;
  }

public: // friends and operator
  T &operator[](size_t index) {
    m_count = algo::max(m_count, index + 1);
    return m_data[index];
  }
  const T &operator[](size_t index) const { return m_data[index]; }

  std::ostream &operator<<(std::ostream &os) const {
    if (m_count == 0) {
      os << "[ ]";
      return os;
    }
    if (m_count == 1) {
      os << "[ " << m_data[0] << " ]";
      return os;
    }
    os << "[ ";
    for (size_t i = 0; i < m_count - 1; i++) {
      os << m_data[i] << ", ";
    }
    os << m_data[m_count - 1] << " ]";
    return os;
  }
  friend void swap(vector &a, vector &b) {
    using std::swap;
    swap(a.m_data, b.m_data);
    swap(a.m_count, b.m_count);
    swap(a.m_capacity, b.m_capacity);
  }

public: // static
  static result<vector, error_enum> make() {
    vector v;
    auto res = allocators::alloc_type_array<A, T>(vector_d::start_cap);
    if (res.has_error()) {
      return {res.take_error()};
    }
    v.m_data = res.take_value();
    v.m_count = 0;
    v.m_capacity = vector_d::start_cap;

    return {std::move(v)};
  }

private:
  T *m_data = nullptr;
  size_t m_count = 0;
  size_t m_capacity = 0;
};

////////

template <typename T, typename A, bool t_is_const> class vector_it {

public:
  using t_coll = vector<T, A>;
  using t_elem = T;
  using t_ptr = std::conditional_t<t_is_const, const t_elem *, t_elem *>;
  using t_ref = std::conditional_t<t_is_const, const t_elem &, t_elem &>;

public:
  vector_it(t_elem *ptr) : m_ptr(ptr) {}
  virtual ~vector_it() = default;
  vector_it(const vector_it &other) = default;
  vector_it &operator=(const vector_it &other) = default;
  vector_it(vector_it &&other) = default;
  vector_it &operator=(vector_it &&other) = default;

public:
  void inc() { m_ptr++; }
  void dec() { m_ptr--; }
  void inc(size_t count) { m_ptr += count; }
  void dec(size_t count) { m_ptr -= count; }
  t_ref get() { return *m_ptr; }
  t_ptr ptr() { return m_ptr; }
  size_t distance(vector_it other) {
    return static_cast<size_t>(algo::abs(other.m_ptr - m_ptr));
  };
  roco::core::ssize_t distance_signed(vector_it other) {
    return (other.m_ptr - m_ptr);
  };

public:
  t_ref operator*() { return *m_ptr; }
  t_ptr operator->() { return m_ptr; }
  bool operator==(const vector_it &other) const { return other.m_ptr == m_ptr; }

public:
  friend void swap(vector_it &a, vector_it &b) { std::swap(a.m_ptr, b.m_ptr); }

private:
  T *m_ptr;
};

/*

template <typename t_elem>
  requires is_collection_elem<t_elem>
class iterator_array_it : public
iterator<t_elem> {
};

template <typename t_elem>
  requires is_collection_elem<t_elem>
uptr<iterator<t_elem>>
into_iterator(array_it<t_elem> &&it,
                                     allocators::allocator
&allocator) {
}
*/
//////////
/*
template <typename t_elem>
  requires is_collection_elem<t_elem>
class collection_view_array : public
collection_view<t_elem> {
};

template <typename t_elem>
uptr<collection_view<t_elem>>
to_collection_view(array<t_elem> &arr,
allocators::allocator &allocator) {
}
*/
///////////

} // namespace collections
} // namespace core
} // namespace roco
