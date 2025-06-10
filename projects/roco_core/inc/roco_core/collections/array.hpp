#pragma once

#include "roco_core/allocators/allocator.hpp"
#include "roco_core/collections/collection.hpp"
#include "roco_core/collections/iterator.hpp"
#include "roco_core/roco_core.hpp"
#include "roco_core/smart_ptr.hpp"

#include <algorithm>
#include <cstddef>
#include <ostream>
#include <utility>

namespace roco {
namespace core {
namespace collections {

template <typename t_elem> class array_it;

template <typename t_elem, size_t t_capacity, typename A>
  requires is_collection_elem<t_elem> && allocators::is_allocator<A>
class array {
public:
  array()
      : m_data(allocators::alloc_type_array<A, t_elem>(t_capacity)),
        m_count(0) {}

  ~array() {
    if (m_data) {
      allocators::delloc_type_array<A, t_elem>(m_data, t_capacity);
      m_data = nullptr;
    }
  }

  array(const array &other) = default;
  array &operator=(const array &other) = default;

  array(array &&other) noexcept {
    this->~array();
    m_data = std::move(other.m_data);
    m_count = std::move(other.m_count);
    other.m_data = nullptr;
    other.m_count = 0;
  }

  array &operator=(array &&other) noexcept {
    array new_array(std::move(other));
    swap(new_array);
    return *this;
  }

public:
  t_elem &operator[](size_t index) {
    if (index + 1 > m_count) {
      m_count = index + 1;
    }
    return m_data[index];
  }
  const t_elem &operator[](size_t index) const { return m_data[index]; }

public:
  size_t count() const noexcept { return m_count; }
  size_t capacity() const noexcept { return t_capacity; }

  void swap(array &other) noexcept {
    std::swap(m_data, other.m_data);
    std::swap(m_count, other.m_count);
  }

public:
  array_it<t_elem> to_array_it_beg() { return array_it<t_elem>(m_data); }
  array_it<t_elem> to_array_it_end() {
    return array_it<t_elem>(m_data + m_count);
  }

public:
  friend std::ostream &operator<<(std::ostream &os, const array &arr)
    requires roco_core::is_printable<t_elem>
  {
    if (arr.m_count == 0) {
      os << "[ ]";
      return os;
    }
    if (arr.m_count == 1) {
      os << "[ " << arr.m_data[0] << " ]";
      return os;
    }
    os << "[ ";
    for (size_t i = 0; i < arr.m_count - 1; i++) {
      os << arr.m_data[i] << ", ";
    }
    os << arr.m_data[arr.m_count - 1] << " ]";
    return os;
  }

private:
  t_elem *m_data = nullptr;
  size_t m_count = 0;
};

///////

template <typename t_elem> class array_it : public iterator<t_elem> {
public:
  array_it(t_elem *ptr) : m_ptr(ptr) {}
  virtual ~array_it() = default;
  array_it(const array_it &other) = default;
  array_it &operator=(const array_it &other) = default;
  array_it(array_it &&other) : m_ptr(other.m_ptr) { other.m_ptr = nullptr; }
  array_it &operator=(array_it &&other) {
    array_it temp(std::move(other));
    swap(temp);
    return *this;
  }

public:
  virtual t_elem &operator*() override { return *m_ptr; }
  virtual void inc() override { m_ptr++; }
  virtual bool equals(const iterator<t_elem> &other) override {
    const array_it *other_array_it = reinterpret_cast<const array_it *>(&other);
    return m_ptr == other_array_it->m_ptr;
  }
  virtual t_elem &get() override { return *m_ptr; }

public:
  void swap(array_it &other) { std::swap(m_ptr, other.m_ptr); }

public:
  template <typename A>
    requires allocators::is_allocator<A>
  roco::core::result<roco::core::uptr<iterator<t_elem>, A>,
                     roco::core::error_enum>
  to_iterator() {
    return {uptr_make<array_it<t_elem>, A>(array_it(*this))};
  }

private:
  t_elem *m_ptr;
};

///////////

} // namespace collections
} // namespace core
} // namespace roco
