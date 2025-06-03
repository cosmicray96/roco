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
namespace collection {

template <typename t_elem> class array_iterator;

template <typename t_elem>
    requires is_collection_elem<t_elem>
class array {
  public:
    array(size_t capacity, allocators::allocator &allocator)
        : m_data((t_elem *)(allocator.alloc(capacity * sizeof(t_elem)))), m_capacity(capacity),
          m_count(0) {}

    ~array() { allocators::registry::dealloc(m_data); }

    array(const array &other) = default;
    array &operator=(const array &other) = default;

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
    array_iterator<t_elem> beg() { return array_iterator<t_elem>(m_data); }
    array_iterator<t_elem> end() { return array_iterator<t_elem>(m_data + m_count); }

  public:
    class dyn : public collection<t_elem> {
      public:
        dyn(array arr) : m_(std::move(arr)) {}
        virtual ~dyn() = default;
        dyn(const dyn &other) = default;
        dyn &operator=(const dyn &other) = default;
        dyn &operator=(dyn &&other) = default;
        dyn(dyn &&other) = default;

      public:
        virtual size_t count() override { return m_->count(); }
        virtual iterator<t_elem> beg() override { return m_->beg().to_iterator(); }
        virtual iterator<t_elem> end() override { return m_->end().to_iterator(); }

      private:
        array m_;
    };

    uptr<collection<t_elem>> to_collection(allocators::allocator &allocator) {
        return make_uptr_dyn<collection<t_elem>, dyn>(allocator, *this);
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

  private:
    void swap(array &other) noexcept {
        std::swap(m_data, other.m_data);
        std::swap(m_capacity, other.m_capacity);
        std::swap(m_count, other.m_count);
    }

  private:
    t_elem *m_data;
    size_t m_capacity;
    size_t m_count;
};

template <typename t_elem> class array_iterator {
  public:
    class dyn : public iterator<t_elem> {
      public:
        dyn(array_iterator arr_it) : m_(std::move(arr_it)) {}
        virtual ~dyn() = default;
        dyn(const dyn &other) = default;
        dyn &operator=(const dyn &other) = default;
        dyn &operator=(dyn &&other) = default;
        dyn(dyn &&other) = default;

      public:
        virtual void inc() override { m_.inc(); }
        virtual bool equals(const iterator<t_elem> &other) override {
            const dyn *other_ptr = dynamic_cast<const dyn *>(&other);
            if (!other_ptr) {
                return false;
            }
            return m_.equals(other_ptr->m_);
        }

        virtual t_elem get() override { return m_.get(); }

      private:
        array_iterator<t_elem> m_;
    };

  public:
    array_iterator(t_elem *ptr) : m_ptr(ptr) {}
    ~array_iterator() = default;
    array_iterator(const array_iterator &other) = default;
    array_iterator &operator=(const array_iterator &other) = default;
    array_iterator &operator=(array_iterator &&other) = default;
    array_iterator(array_iterator &&other) = default;

  public:
    void inc() { m_ptr++; }
    bool equals(const array_iterator<t_elem> &other) { return m_ptr == other.m_ptr; }
    t_elem get() { return *m_ptr; }

  public:
    uptr<iterator<t_elem>> to_iterator(allocators::allocator &allocator) {
        return make_uptr_dyn<iterator<t_elem>, dyn>(allocator, *this);
    }

  private:
    t_elem *m_ptr;
};

} // namespace collection
} // namespace core
} // namespace roco
