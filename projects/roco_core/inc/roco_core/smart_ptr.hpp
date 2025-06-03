#pragma once

#include "roco_core/allocators/allocator.hpp"
#include <utility>

namespace roco {
namespace core {

template <typename T> class uptr {
public:
  template <typename... Args>
  uptr(allocators::allocator &allocator, Args &&...args)
      : m_(allocator.alloc(sizeof(T))) {
    new (m_) T(std::forward<Args>(args)...);
  }
  ~uptr() {
    if (m_) {
      m_->~T();
      allocators::registry::dealloc(m_);
    }
  }

  uptr(const uptr &other) = delete;
  uptr &operator=(const uptr &other) = delete;

  uptr(uptr &&other) : m_(other.m_) { other.m_ = nullptr; }
  uptr &operator=(uptr &&other) {
    uptr temp(std::move(other));
    swap(temp);
    return *this;
  }

public:
  T *operator->() { return m_; }
  T *get() { return m_; }

private:
  void swap(uptr &other) { std::swap(m_, other.m_); }

private:
  T *m_;
};

} // namespace core
} // namespace roco
