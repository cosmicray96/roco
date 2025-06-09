#pragma once

#include "roco_core/allocators/allocator.hpp"
#include <concepts>
#include <type_traits>
#include <utility>

namespace roco {
namespace core {

template <typename T, typename A>
    requires allocators::is_allocator<A>
class uptr {
  public:
    uptr() = default;
    ~uptr() { destroy(); }

    uptr(const uptr &other) = delete;
    uptr &operator=(const uptr &other) = delete;

    uptr(uptr &&other) {
        using std::swap;
        swap(*this, other);
    }

    uptr &operator=(uptr &&other) {
        destroy();
        swap(*this, other);
        return *this;
    }

  public:
    template <typename U>
        requires std::is_base_of_v<T, U>
    uptr(uptr<U, A> &&other) : m_(static_cast<T *>(other.m_)), m_delete_fn(other.m_delete_fn) {
        other.m_ = nullptr;
        other.m_delete_fn = nullptr;
    }

    template <typename U>
        requires std::is_base_of_v<T, U>
    uptr &operator=(uptr<U, A> &&other) {
        destroy();
        m_ = static_cast<T *>(other.m_);
        m_delete_fn = other.m_delete_fn;
        other.m_ = nullptr;
        other.m_delete_fn = nullptr;
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

    void destroy() {
        if (m_) {
            m_delete_fn(m_);
            m_ = nullptr;
            m_delete_fn = nullptr;
        }
    }

  public:
    friend void swap(uptr &a, uptr &b) {
        using std::swap;
        swap(a.m_, b.m_);
        swap(a.m_delete_fn, b.m_delete_fn);
    }

  public:
    template <typename _T, typename _A>
        requires allocators::is_allocator<_A>
    friend uptr<_T, _A> uptr_take(_T *ptr);

    template <typename _T, typename _A, typename... _Args>
        requires allocators::is_allocator<_A>
    friend uptr<_T, _A> uptr_make(_Args &&...args);

  private:
    T *m_ = nullptr;
    void (*m_delete_fn)(void *) = nullptr;
};

template <typename T, typename A>
    requires allocators::is_allocator<A>
uptr<T, A> uptr_take(T *ptr) {
    uptr<T, A> p;
    p.m_ = ptr;
    p.m_delete_fn = [](void *ptr) { allocators::dealloc_type<A>(static_cast<T *>(ptr)); };
    return p;
}

template <typename T, typename A, typename... Args>
    requires allocators::is_allocator<A>
uptr<T, A> uptr_make(Args &&...args) {
    uptr<T, A> ptr;
    ptr.m_ = allocators::alloc_type<A, T, Args...>(std::forward<Args>(args)...);
    ptr.m_delete_fn = [](void *ptr) { allocators::dealloc_type<A>(static_cast<T *>(ptr)); };
    return ptr;
}

//////

template <typename T, typename A>
    requires allocators::is_allocator<A>
class sptr;

template <typename A>
    requires allocators::is_allocator<A>
class sptr_ctx {
  public:
    sptr_ctx() = default;
    ~sptr_ctx() { destroy(); }

    sptr_ctx(const sptr_ctx &other) = delete;
    sptr_ctx &operator=(const sptr_ctx &other) = delete;
    sptr_ctx(sptr_ctx &&other) = delete;
    sptr_ctx &operator=(sptr_ctx &&other) = delete;

  public:
    void *get() { return m_; }

    void increment_ref() { m_ref_count++; }
    void decrement_ref() {
        m_ref_count--;
        if (m_ref_count <= 0) {
            destroy();
        }
    }
    void destroy() {
        if (m_) {
            m_delete_fn(m_);
            m_ = nullptr;
            m_delete_fn = nullptr;
        }
    }

  public:
    template <typename _T, typename _A, typename... _Args>
        requires allocators::is_allocator<_A>
    friend sptr<_T, _A> sptr_make(_Args &&...args);

  private:
    void *m_ = nullptr;
    void (*m_delete_fn)(void *) = nullptr;
    size_t m_ref_count = 0;
};

template <typename T, typename A>
    requires allocators::is_allocator<A>
class sptr {
  public:
    sptr() = default;
    ~sptr() { destroy(); }

    sptr(const sptr &other) : m_ctx(other.m_ctx) { m_ctx->increment_ref(); }
    sptr &operator=(const sptr &other) {
        destroy();
        using std::swap;
        swap(*this, other);
        return *this;
    }

    sptr(sptr &&other) : m_ctx(std::move(other.m_ctx)) { other.m_ctx = nullptr; }
    sptr operator=(sptr &&other) {
        destroy();
        using std::swap;
        swap(*this, other);
        return *this;
    }

    template <typename U>
        requires std::is_base_of_v<T, U>
    sptr(const sptr<U, A> &other) : m_ctx(other.m_ctx) {
        m_ctx->increment_ref();
    }

    template <typename U>
        requires std::is_base_of_v<T, U>
    sptr &operator=(const sptr<U, A> &other) {
        destroy();
        m_ctx = other.m_ctx;
        m_ctx->increment_ref();
        return *this;
    }

    template <typename U>
        requires std::is_base_of_v<T, U>
    sptr(sptr &&other) : m_ctx(other.m_ctx) {
        other.m_ctx = nullptr;
    }

    template <typename U>
        requires std::is_base_of_v<T, U>
    sptr &operator=(sptr &&other) {
        destroy();
        m_ctx = other.m_ctx;
        other.m_ctx = nullptr;
    }

  public:
    T *operator->() { return static_cast<T *>(m_ctx->get()); }

    void destroy() {
        if (m_ctx) {
            m_ctx->decrement_ref();
            m_ctx = nullptr;
        }
    }

  public:
    friend void swap(sptr &a, sptr &b) { std::swap(a.m_ctx, b.m_ctx); }

    template <typename _T, typename _A, typename... _Args>
        requires allocators::is_allocator<_A>
    friend sptr<_T, _A> sptr_make(_Args &&...args);

    template <typename _T, typename _A>
        requires allocators::is_allocator<_A>
    friend sptr<_T, _A> sptr_take(_T *ptr);

  public:
    sptr_ctx<A> *m_ctx;
};

template <typename T, typename A, typename... Args>
    requires allocators::is_allocator<A>
sptr<T, A> sptr_make(Args &&...args) {
    sptr<T, A> ptr;
    sptr_ctx<A> *ctx = allocators::alloc_type<A, sptr_ctx<A>>();
    ctx->m_ = allocators::alloc_type<A, T>(std::forward<Args>(args)...);
    ctx->m_delete_fn = [](void *ptr) { allocators::dealloc_type<A>(static_cast<T *>(ptr)); };
    ctx->m_ref_count = 1;
    ptr.m_ctx = ctx;
    return ptr;
}

template <typename T, typename A>
    requires allocators::is_allocator<A>
sptr<T, A> sptr_take(T *ptr) {
    sptr<T, A> p;
    sptr_ctx<A> *ctx = allocators::alloc_type<A, sptr_ctx<A>>();
    ctx->m_ = ptr;
    ctx->m_delete_fn = [](void *ptr) { allocators::dealloc_type<A>(static_cast<T *>(ptr)->~T()); };
    ctx->m_ref_count = 1;
    p.m_ctx = ctx;
    return p;
}

} // namespace core
} // namespace roco
