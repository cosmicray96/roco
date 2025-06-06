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

template <typename t_elem>
    requires is_collection_elem<t_elem>
class vec {
  public:
    vec(allocators::allocator &allocator) : m_capacity(10), m_count(0), m_allocator(allocator) {
        m_data = (t_elem *)(allocator.alloc(m_capacity * sizeof(t_elem)));
    }
    ~vec() {
        if (m_data) {
            m_allocator.dealloc(m_data);
            m_data = nullptr;
        }
    }

    vec(const vec &other) = default;
    vec &operator=(const vec &other) = default;

    vec(vec &&other) : m_data(other.m_data), m_capacity(other.m_capacity), m_count(other.m_count) {}

  private:
    void swap(vec &other) {
        std::swap(m_data, other.m_data);
        std::swap(m_capacity, other.m_capacity);
        std::swap(m_count, other.m_count);
        allocators::allocator &temp = other.m_allocator;
        other.m_allocator = m_allocator;
        m_allocator = temp;
    }

  public:
    t_elem *m_data;
    size_t m_capacity;
    size_t m_count;
    allocators::allocator &m_allocator;
};

template <typename t_elem> class vec_it {};

template <typename t_elem>
    requires is_collection_elem<t_elem>
vec_it<t_elem> to_vec_it_beg(vec<t_elem> &ve) {
    return vec_it<t_elem>(ve.m_data);
}
template <typename t_elem>
    requires is_collection_elem<t_elem>
vec_it<t_elem> to_vec_it_end(vec<t_elem> &ve) {
    return vec_it<t_elem>(ve.m_data + ve.m_count);
}

////////////

//////////

/*

template <typename t_elem>
  requires is_collection_elem<t_elem>
class iterator_array_it : public iterator<t_elem> {
};

template <typename t_elem>
  requires is_collection_elem<t_elem>
uptr<iterator<t_elem>> into_iterator(array_it<t_elem> &&it,
                                     allocators::allocator &allocator) {
}
*/
//////////
/*
template <typename t_elem>
  requires is_collection_elem<t_elem>
class collection_view_array : public collection_view<t_elem> {
};

template <typename t_elem>
uptr<collection_view<t_elem>>
to_collection_view(array<t_elem> &arr, allocators::allocator &allocator) {
}
*/
///////////

} // namespace collections
} // namespace core
} // namespace roco
