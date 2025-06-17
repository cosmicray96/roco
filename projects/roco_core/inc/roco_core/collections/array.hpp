#pragma once

#include "roco_core/allocators/allocator.hpp"
#include "roco_core/collections/collection.hpp"
#include "roco_core/collections/iterator.hpp"
#include "roco_core/error_enum.hpp"
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

template <typename T, size_t N, typename A> class array {
  private:
    array() = default;

  public:
    ~array() { destroy(); }

    array(const array &other) = default;
    array &operator=(const array &other) = default;

    array(array &&other) { swap(*this, other); }
    array &operator=(array &&other) {
        destroy();
        swap(*this, other);
        return *this;
    }

  public:
    T &operator[](size_t index) {
        if (index + 1 > m_count) {
            m_count = index + 1;
        }
        return m_data[index];
    }
    const T &operator[](size_t index) const { return m_data[index]; }

  public:
    size_t count() const { return m_count; }
    size_t capacity() const { return N; }

    void destroy() {
        if (m_data) {
            allocators::delloc_type_array<A, T>(m_data, N);
            m_data = nullptr;
        }
    }

  public:
    array_it<T> to_array_it_beg() { return array_it<T>(m_data); }
    array_it<T> to_array_it_end() { return array_it<T>(m_data + m_count); }

  public:
    friend void swap(array &a, array &b) noexcept {
        std::swap(a.m_data, b.m_data);
        std::swap(a.m_count, b.m_count);
    }

    friend std::ostream &operator<<(std::ostream &os, const array &arr)
        requires roco::core::is_printable<T>
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

  public:
    static roco::core::result<array, roco::core::error_enum> make()
        requires is_collection_elem<T> && allocators::is_allocator<A>;

  private:
    T *m_data = nullptr;
    size_t m_count = 0;
};

template <typename T, size_t N, typename A>
roco::core::result<array<T, N, A>, roco::core::error_enum> array<T, N, A>::make()
    requires is_collection_elem<T> && allocators::is_allocator<A>
{
    array<T, N, A> arr;
    roco::core::result<T *, roco::core::error_enum> res = allocators::alloc_type_array<A, T>(N);

    if (res.has_error()) {
        return {res.take_error()};
    }
    arr.m_data = res.take_value();
    return {arr};
}
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
    roco::core::result<roco::core::uptr<iterator<t_elem>, A>, roco::core::error_enum>
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
