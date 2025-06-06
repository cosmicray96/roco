#pragma once

#include "roco_core/allocators/allocator.hpp"
#include <type_traits>
#include <utility>

namespace roco {
namespace core {

template <typename T, typename A>
    requires allocators::is_allocator<A>
class uptr {
  public:
    uptr() : m_(nullptr), m_delete_fn(nullptr) {}

  public:
    ~uptr() {
        if (m_) {
            m_delete_fn(m_);
            A::dealloc(m_);
        }
    }

    uptr(const uptr &other) = delete;
    uptr &operator=(const uptr &other) = delete;

    uptr(uptr &&other) : m_(other.m_), m_delete_fn(other.m_delete_fn) {
        other.m_ = nullptr;
        other.m_delete_fn = nullptr;
    }
    uptr &operator=(uptr &&other) {
        uptr temp(std::move(other));
        swap(temp);
        return *this;
    }

  public:
    T *operator->() { return m_; }
    T *get() { return m_; }

    T *release() {
        T *temp = m_;
        m_ = nullptr;
        m_delete_fn = nullptr;
        return temp;
    }

  public:
    void swap(uptr &other) {
        std::swap(m_, other.m_);
        std::swap(m_delete_fn, other.m_delete_fn);
    }

  public:
    T *m_;
    void (*m_delete_fn)(T *);
};

template <typename T, typename A, typename... Args>
    requires allocators::is_allocator<A>
uptr<T, A> make_uptr(Args &&...args) {
    uptr<T, A> ptr;
    ptr.m_ = A::template alloc<T>(std::forward<Args>(args)...);
    ptr.m_delete_fn = [](T *ptr) { ptr->~T(); };
    return ptr;
}

template <typename T, typename U, typename A, typename... Args>
    requires std::is_base_of_v<T, U> && allocators::is_allocator<A>
uptr<T, A> make_uptr_dyn(Args &&...args) {
    uptr<T, A> ptr;
    ptr.m_ = reinterpret_cast<T *>(A::template alloc<U>(std::forward<Args>(args)...));
    ptr.m_delete_fn = [](T *ptr) { reinterpret_cast<U *>(ptr)->~U(); };
    return ptr;
}

//////
/*
template <typename T> class sptr_ctx {
  public:
    void decrement_ref() {
        m_ref_count--;
        if (m_ref_count <= 0) {
            ~sptr_ctx();
        }
    }

  public:
    // this should store allocator here
    T *m_ptr = nullptr;
    size_t m_ref_count = 0;
};

template <typename T> class sptr {
  public:
    sptr() {
        m_ctx = reinterpret_cast<sptr_ctx<T>
*>(allocator.alloc(sizeof(sptr_ctx<T>)));
    }
    ~sptr() {
        if (m_ctx) {
            m_ctx->m_ref_count--;
        }
    }

    sptr(const sptr &other) : m_ctx(other.m_ctx) { m_ctx->m_ref_count++; }
    sptr &operator=(const sptr &other) {
        sptr temp(other);
        swap(temp);
        return *this;
    }

    sptr(sptr &&other) : m_ctx(std::move(other.m_ctx)) {}
    sptr operator=(sptr &&other) {
        sptr temp(other);
        swap(temp);
        return *this;
    }

  public:
    void swap(sptr_ctx<T> &other) { std::swap(m_ctx, other.m_ctx); }

  public:
    sptr_ctx<T> *m_ctx;
};

template <typename T, typename U, typename... Args>
    requires std::is_base_of_v<T, U>
sptr<T, A> make_sptr_dyn(allocators::allocator &allocator, Args &&...args) {
    sptr<T, A> ptr;

    void *temp = allocator.alloc(sizeof(U));
    U *temp2 = new (temp) U(std::forward(args)...);
    ptr.m_ctx.m_ptr = reinterpret_cast<T *>(temp2);
    ptr.m_ctx.m_ref_count = 1;
    return ptr;
}
*/
} // namespace core
} // namespace roco
