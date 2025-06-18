#pragma once

#include "roco_core/allocators/allocator.hpp"
#include "roco_core/error.hpp"
#include "roco_core/error_enum.hpp"
#include <concepts>
#include <type_traits>
#include <utility>

namespace roco {
namespace core {

template <typename T, typename A> class uptr {
  public:
    static_assert(allocators::is_allocator<A>, "A is not allocator");

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
        if (m_ == other.m_) {
            return *this;
        }
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
    T &operator*() { return *m_; }
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

    template <typename _T, typename _A> friend class uptr;

  public:
    static result<uptr, error_enum> take(T *ptr) {
        if (!ptr) {
            return {error_enum::is_null_pointer};
        }
        uptr p;
        p.m_ = ptr;
        p.m_delete_fn = [](void *ptr) { allocators::dealloc_type<A>(static_cast<T *>(ptr)); };
        return {std::move(p)};
    }

    template <typename... Args> static result<uptr, error_enum> make(Args &&...args) {
        uptr ptr;
        result<T *, error_enum> res =
            allocators::alloc_type<A, T, Args...>(std::forward<Args>(args)...);
        if (res.has_error()) {
            return {res.take_error()};
        }
        ptr.m_ = res.take_value();
        ptr.m_delete_fn = [](void *ptr) { allocators::dealloc_type<A>(static_cast<T *>(ptr)); };
        return {std::move(ptr)};
    }

  private:
    T *m_ = nullptr;
    void (*m_delete_fn)(void *) = nullptr;
};

//////

template <typename T, typename A> class sptr;

template <typename A> class sptr_ctx {
  public:
    static_assert(allocators::is_allocator<A>, "A is not allocator");

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
    template <typename T> friend class sptr;

  private:
    void *m_ = nullptr;
    void (*m_delete_fn)(void *) = nullptr;
    size_t m_ref_count = 0;
};

template <typename T, typename A> class sptr {
  public:
    static_assert(allocators::is_allocator<A>, "A is not allocator");

  private:
    sptr() = default;

  public:
    ~sptr() { destroy(); }

    sptr(const sptr &other) : m_ctx(other.m_ctx) { m_ctx->increment_ref(); }
    sptr &operator=(const sptr &other) {
        if (m_ctx == other.m_ctx) {
            return *this;
        }
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

    template <typename _T, typename _A> friend class sptr;

  public:
    template <typename... Args> static result<sptr<T, A>, error_enum> make(Args &&...args) {
        sptr ptr;
        auto res = allocators::alloc_type<A, sptr_ctx<A>>();
        if (res.has_error()) {
            return {res.take_error()};
        }
        sptr_ctx<A> *ctx = res.take_value();
        auto res1 = allocators::alloc_type<A, T>(std::forward<Args>(args)...);
        if (res1.has_error()) {
            return {res1.take_error()};
        }
        ctx->m_ = res1.take_value();
        ctx->m_delete_fn = [](void *ptr) { allocators::dealloc_type<A>(static_cast<T *>(ptr)); };
        ctx->m_ref_count = 1;
        ptr.m_ctx = ctx;
        return {std::move(ptr)};
    }

    static result<sptr, error_enum> take(T *ptr) {
        if (!ptr) {
            return {error_enum::is_null_pointer};
        }
        sptr<T, A> p;
        result<sptr_ctx<A> *, error_enum> res = allocators::alloc_type<A, sptr_ctx<A>>();
        if (res.has_error()) {
            return {res.take_error()};
        }
        sptr_ctx<A> *ctx = res.take_value();
        ctx->m_ = ptr;
        ctx->m_delete_fn = [](void *ptr) {
            allocators::dealloc_type<A>(static_cast<T *>(ptr)->~T());
        };
        ctx->m_ref_count = 1;
        p.m_ctx = ctx;
        return {std::move(p)};
    }

  public:
    sptr_ctx<A> *m_ctx;
};

} // namespace core
} // namespace roco
