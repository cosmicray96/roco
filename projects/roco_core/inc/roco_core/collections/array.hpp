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
class array {
  public:
    array(size_t capacity, allocators::allocator &allocator)
        : m_data((t_elem *)(allocator.alloc(capacity * sizeof(t_elem)))), m_capacity(capacity),
          m_count(0) {}

    ~array() {
        if (m_data) {
            allocators::registry::dealloc(m_data);
            m_data = nullptr;
        }
    }

    array(const array &other) = delete;
    array &operator=(const array &other) = delete;

    array(array &&other) noexcept
        : m_data(other.m_data), m_capacity(other.m_capacity), m_count(other.m_count) {
        other.m_data = nullptr;
        other.m_capacity = 0;
        other.m_count = 0;
    }

    array &operator=(array &&other) noexcept {
        array new_array(std::move(other));
        swap(new_array);
        return *this;
    }

  public:
    static array<t_elem> copy(const array<t_elem> &other, allocators::allocator &a) {
        array<t_elem> new_array(other.m_capacity, a);
        new_array.m_count = other.m_count;
        memcpy(new_array.m_data, other.m_data, new_array.m_capacity * sizeof(t_elem));
        return new_array;
    }

  public:
    t_elem &operator[](size_t index) {

        if (index + 1 > m_count) {
            m_count = index + 1;
        }
        return m_data[index];
    }
    const t_elem &operator[](size_t index) const { return m_data[index]; }

    friend std::ostream &operator<<(std::ostream &os, const array<t_elem> &arr)
        requires roco_core::is_printable<t_elem>
    {
        if (arr.m_count == 0) {
            os << "[ ]";
            return os;
        }
        if (arr.m_count == 1) {
            os << "[ " << arr[0] << " ]";
            return os;
        }
        os << "[ ";
        for (size_t i = 0; i < arr.m_count - 1; i++) {
            os << arr[i] << ", ";
        }
        os << arr[arr.m_count - 1] << " ]";
        return os;
    }

  public:
    size_t count() const noexcept { return m_count; }
    size_t capacity() const noexcept { return m_capacity; }

  private:
    void swap(array &other) noexcept {
        std::swap(m_data, other.m_data);
        std::swap(m_capacity, other.m_capacity);
        std::swap(m_count, other.m_count);
    }

  public:
    t_elem *m_data;
    size_t m_capacity;
    size_t m_count;
};

template <typename t_elem> class array_it {
  public:
    array_it(t_elem *ptr) : m_ptr(ptr) {}
    ~array_it() = default;
    array_it(const array_it &other) = default;
    array_it &operator=(const array_it &other) = default;
    array_it(array_it &&other) : m_ptr(other.m_ptr) { other.m_ptr = nullptr; }
    array_it &operator=(array_it &&other) {
        array_it temp(std::move(other));
        swap(temp);
        return *this;
    }

  public:
    t_elem &operator*() { return *m_ptr; }
    void inc() { m_ptr++; }
    bool equals(const array_it<t_elem> &other) { return m_ptr == other.m_ptr; }
    t_elem get() { return *m_ptr; }

  private:
    void swap(array_it &other) { std::swap(m_ptr, other.m_ptr); }

  public:
    t_elem *m_ptr;
};

template <typename t_elem>
    requires is_collection_elem<t_elem>
array_it<t_elem> to_array_it_beg(array<t_elem> &arr) {
    return array_it<t_elem>(arr.m_data);
}
template <typename t_elem>
    requires is_collection_elem<t_elem>
array_it<t_elem> to_array_it_end(array<t_elem> &arr) {
    return array_it<t_elem>(arr.m_data + arr.m_count);
}

////////////

//////////

template <typename t_elem>
    requires is_collection_elem<t_elem>
class iterator_array_it : public iterator<t_elem> {
  public:
    iterator_array_it(array_it<t_elem> &&arr_it) : m_(std::move(arr_it)) {}
    virtual ~iterator_array_it() = default;
    iterator_array_it(const iterator_array_it &other) = default;
    iterator_array_it &operator=(const iterator_array_it &other) = default;
    iterator_array_it &operator=(iterator_array_it &&other) = default;
    iterator_array_it(iterator_array_it &&other) = default;

  public:
    virtual t_elem &operator*() override { return *m_; }
    virtual void inc() override { m_.inc(); }
    virtual bool equals(const iterator<t_elem> &other) override {
        const iterator_array_it *other_ptr = dynamic_cast<const iterator_array_it *>(&other);
        if (!other_ptr) {
            return false;
        }
        return m_.equals(other_ptr->m_);
    }

    virtual t_elem get() override { return m_.get(); }

  public:
    array_it<t_elem> m_;
};

template <typename t_elem>
    requires is_collection_elem<t_elem>
uptr<iterator<t_elem>> into_iterator(array_it<t_elem> &&it, allocators::allocator &allocator) {
    uptr<iterator<t_elem>> output =
        make_uptr_dyn<iterator<t_elem>, iterator_array_it<t_elem>>(allocator, std::move(it));
    return output;
}

//////////?

template <typename t_elem>
    requires is_collection_elem<t_elem>
class collection_view_array : public collection_view<t_elem> {
  public:
    collection_view_array(array<t_elem> &arr) : m_(arr) {}
    virtual ~collection_view_array() = default;
    collection_view_array(const collection_view_array &other) = default;
    collection_view_array &operator=(const collection_view_array &other) = default;
    collection_view_array &operator=(collection_view_array &&other) = default;
    collection_view_array(collection_view_array &&other) = default;

  public:
    virtual size_t count() override { return m_.count(); }
    virtual uptr<iterator<t_elem>> beg(allocators::allocator &a) override {
        return into_iterator(to_array_it_beg(m_), a);
    }
    virtual uptr<iterator<t_elem>> end(allocators::allocator &a) override {
        return into_iterator(to_array_it_end(m_), a);
    }

  public:
    array<t_elem> &m_;
};

template <typename t_elem>
uptr<collection_view<t_elem>> to_collection_view(array<t_elem> &arr,
                                                 allocators::allocator &allocator) {
    return make_uptr_dyn<collection_view<t_elem>, collection_view_array<t_elem>>(allocator, arr);
}

///////////

} // namespace collections
} // namespace core
} // namespace roco
