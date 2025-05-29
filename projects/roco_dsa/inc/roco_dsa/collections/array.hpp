#pragma once

#include "roco_dsa/allocators/allocator.hpp"
#include "roco_dsa/collections/collection.hpp"

#include <concepts>
#include <cstddef>
#include <utility>

namespace roco {
namespace dsa {
namespace collection {

template <typename T, typename A>
  requires is_collection_elem<T> && is_allocator<A>
class array {
public:
  using t_elem = T;

public:
  array(size_t capacity)
      : m_data((T *)(A::alloc(capacity * sizeof(T)))), m_capacity(capacity),
        m_count(0) {}

  ~array() { A::dealloc(m_data); }

  array(const array &other)
      : m_capacity(other.m_capacity), m_count(other.m_count) {
    m_data = A::alloc(m_capacity * sizeof(T));
    memcpy(m_data, other.m_data, m_capacity * sizeof(T));
  }

  array &operator=(const array &other) {
    array new_array(other);
    swap(new_array);
    return *this;
  }

  array(array &&other) noexcept
      : m_data(other.m_data), m_capacity(other.m_capacity),
        m_count(other.m_count) {
    other.m_data = nullptr;
    other.m_capacity = 0;
    other.m_count = 0;
  }

  array &operator=(array &&other) noexcept {
    array new_array(std::move(other));
    swap(new_array);
    return *this;
  }

private:
  void swap(array &other) noexcept {
    std::swap(m_data, other.m_data);
    std::swap(m_capacity, other.m_capacity);
    std::swap(m_count, other.m_count);
  }

private:
  T *m_data;
  size_t m_capacity;
  size_t m_count;
};

} // namespace collection
} // namespace dsa
} // namespace roco
