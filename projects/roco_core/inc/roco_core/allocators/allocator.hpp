#pragma once

#include <concepts>
#include <cstddef>
#include <utility>

#include "roco_core/error.hpp"
#include "roco_core/error_enum.hpp"

namespace roco {
namespace core {
namespace allocators {

template <typename A>
concept is_allocator = requires(A a, size_t size, size_t align, void *ptr) {
  {
    A::alloc(size, align)
  } -> std::same_as<roco::core::result<void *, roco::core::error_enum>>;
  {
    A::alloc8(size)
  } -> std::same_as<roco::core::result<void *, roco::core::error_enum>>;
  {
    A::alloc16(size)
  } -> std::same_as<roco::core::result<void *, roco::core::error_enum>>;
  { A::dealloc(ptr) } -> std::same_as<void>;
};

constexpr size_t aligned_size(size_t size, size_t align) {
  return ((size + align - 1) / align) * align;
}
template <typename T> constexpr size_t aligned_size_type() {
  constexpr size_t size = sizeof(T);
  constexpr size_t align = alignof(T);
  return aligned_size(size, align);
}

template <typename A, typename T, typename... Args>
  requires is_allocator<A>
result<T *, roco::core::error_enum> alloc_type(Args &&...args) {
  result<void *, roco::core::error_enum> res = A::alloc(sizeof(T), alignof(T));
  if (res.has_error()) {
    return {res.take_error()};
  }
  T *output = new (res.take_value()) T(std::forward<Args>(args)...);
  if (!output) {
    return {roco::core::error_enum::alloc_bad_alloc};
  }
  return {output};
}

template <typename A, typename T>
  requires is_allocator<A>
optional<roco::core::error_enum> dealloc_type(T *ptr) {
  if (!ptr) {
    return {roco::core::error_enum::alloc_bad_dealloc};
  }
  ptr->~T();
  A::dealloc(ptr);
  return {};
}

template <typename A, typename T, typename... Args>
  requires is_allocator<A>
result<T *, roco::core::error_enum> alloc_type_array(size_t count,
                                                     Args &&...args) {
  size_t padded_size = aligned_size(sizeof(T) * count, alignof(T));
  result<void *, roco::core::error_enum> res =
      A::alloc(padded_size, alignof(T));
  if (res.has_error()) {
    return {res.take_error()};
  }
  std::byte *ptr = static_cast<std::byte *>(res.take_value());
  for (size_t i = 0; i < count; i++) {
    void *temp = ptr + (i * sizeof(T));
    new (temp) T(std::forward<Args>(args)...);
  }
  return {static_cast<T *>(ptr)};
}

template <typename A, typename T>
  requires is_allocator<A>
optional<roco::core::error_enum> delloc_type_array(T *ptr, size_t count) {
  if (!ptr) {
    return {roco::core::error_enum::alloc_bad_dealloc};
  }
  const size_t al_size = aligned_size_type<T>();
  const std::byte *temp_ptr = static_cast<T>(ptr);
  for (size_t i = 0; i < count; i++) {
    T *temp = static_cast<T *>(temp_ptr + (i * al_size));
    temp->~T();
  }
  A::dealloc(ptr);
  return {};
}

} // namespace allocators
} // namespace core
} // namespace roco
