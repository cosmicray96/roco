#pragma once

#include <concepts>
#include <cstddef>
#include <cstring>
#include <memory>
#include <utility>

#include "roco_core/error.hpp"
#include "roco_core/error_enum.hpp"
#include "roco_core/roco_core.hpp"

namespace roco {
namespace core {
namespace allocators {

enum class allocator_type { u16, u32, u64, raw };

class allocator {
protected:
  allocator() = default;

public:
  virtual ~allocator() = default;

public:
  virtual allocator_type type_of() = 0;

public:
  allocator(const allocator &other) = delete;
  allocator operator=(const allocator &other) = delete;
  allocator(allocator &&other) = delete;
  allocator operator=(allocator &&other) = delete;
};

class allocator16 : public allocator {
public:
  using t_handle = uint16_t;

protected:
  allocator16() : allocator() {}

public:
  virtual ~allocator16() = default;

public:
  virtual allocator_type type_of() override { return allocator_type::u16; }

public:
  virtual result<uint16_t, error_enum> alloc(uint16_t size, uint16_t align) = 0;
  virtual void dealloc(uint16_t size) = 0;
  virtual void *access(uint16_t handle) = 0;
};

class allocator32 : public allocator {
public:
  using t_handle = uint32_t;

protected:
  allocator32() : allocator() {}

public:
  virtual ~allocator32() = default;

public:
  virtual allocator_type type_of() override { return allocator_type::u32; }

public:
  virtual result<uint32_t, error_enum> alloc(uint32_t size, uint32_t align) = 0;
  virtual void dealloc(uint32_t size) = 0;
  virtual void *access(uint32_t handle) = 0;
};

class allocator64 : public allocator {
public:
  using t_handle = uint64_t;

protected:
  allocator64() : allocator() {}

public:
  virtual ~allocator64() = default;

public:
  virtual allocator_type type_of() override { return allocator_type::u64; }

public:
  virtual result<uint64_t, error_enum> alloc(uint64_t size, uint64_t align) = 0;
  virtual void dealloc(uint64_t size) = 0;
  virtual void *access(uint64_t handle) = 0;
};
class allocator_raw : public allocator {
public:
  using t_handle = void *;

protected:
  allocator_raw() : allocator() {}

public:
  virtual ~allocator_raw() = default;

public:
  virtual allocator_type type_of() override { return allocator_type::raw; }

public:
  virtual result<void *, error_enum> alloc(size_t size, size_t align) = 0;
  virtual void dealloc(void *ptr) = 0;
};

template <typename A>
concept is_allocator16 = requires(A a, uint16_t size, uint16_t align) {
  requires std::same_as<typename A::t_handle, uint16_t>;
  { a.alloc(size, align) } -> std::same_as<result<uint16_t, error_enum>>;
  { a.dealloc(size) } -> std::same_as<void>;
};

template <typename A>
concept is_allocator32 = requires(A a, uint32_t size, uint32_t align) {
  requires std::same_as<typename A::t_handle, uint32_t>;
  { a.alloc(size, align) } -> std::same_as<result<uint32_t, error_enum>>;
  { a.dealloc(size) } -> std::same_as<void>;
};

template <typename A>
concept is_allocator64 = requires(A a, uint64_t size, uint64_t align) {
  requires std::same_as<typename A::t_handle, uint64_t>;
  { a.alloc(size, align) } -> std::same_as<result<uint64_t, error_enum>>;
  { a.dealloc(size) } -> std::same_as<void>;
};

template <typename A>
concept is_allocator_raw = requires(A a, size_t size, size_t align, void *ptr) {
  requires std::same_as<typename A::t_handle, void *>;
  { a.alloc(size, align) } -> std::same_as<result<void *, error_enum>>;
  { a.dealloc(ptr) } -> std::same_as<void>;
};

template <typename A>
concept is_allocator =
    std::same_as<A, allocator> || is_allocator16<A> || is_allocator32<A> ||
    is_allocator64<A> || is_allocator_raw<A>;

static_assert(is_allocator<allocator16>);
static_assert(is_allocator<allocator32>);
static_assert(is_allocator<allocator64>);
static_assert(is_allocator<allocator_raw>);
static_assert(is_allocator16<allocator16>);
static_assert(is_allocator32<allocator32>);
static_assert(is_allocator64<allocator64>);
static_assert(is_allocator_raw<allocator_raw>);

constexpr size_t aligned_size(size_t size, size_t align) {
  return ((size + align - 1) / align) * align;
}
template <typename T> constexpr size_t aligned_size_type() {
  constexpr size_t size = sizeof(T);
  constexpr size_t align = alignof(T);
  return aligned_size(size, align);
}

namespace _details {

template <typename A, typename T, typename... Args>
  requires is_allocator<A>
result<typename A::t_handle, error_enum> alloc_type(A &allocator,
                                                    Args &&...args) {
  switch (allocator.type_of()) {
  case allocator_type::raw: {
    auto res =
        dynamic_cast<allocator_raw>(allocator).alloc(sizeof(T), alignof(T));
    if (res.has_error()) {
      return {res.take_error()};
    }
    typename A::t_handle handle = res.take_value();
    std::construct_at(reinterpret_cast<T *>(handle),
                      std::forward<Args>(args)...);
    if (!handle) {
      return {error_enum::alloc_bad_alloc};
    }
    return {std::move(handle)};
    break;
  }
  default: {
    return {error_enum::error};
    break;
  }
  }
}

template <typename A, typename T>
  requires is_allocator<A>
void dealloc_type(A &allocator, typename A::t_handle handle) {

  switch (allocator.type_of()) {
  case allocator_type::raw: {
    if (!handle) {
      crash_program(error_enum::is_null_pointer, "trying to free nullptr");
    }
    std::destroy_at(handle);
    A::dealloc(handle);
    break;
  }
  default: {
    crash_program("ERROR");
  }
  }
}

template <typename A, typename T, typename... Args>
result<typename A::t_handle, error_enum>
alloc_type_array(A &allocator, size_t count, Args &&...args) {
  if (count == 0) {
    return {roco::core::error_enum::alloc_array_bad_size};
  }
  size_t padded_size = aligned_size(sizeof(T) * count, alignof(T));
  switch (allocator.type_of()) {
  case allocator_type::raw: {
    auto res =
        dynamic_cast<allocator_raw>(allocator).alloc(padded_size, alignof(T));
    if (res.has_error()) {
      return {res.take_error()};
    }
    std::byte *ptr = reinterpret_cast<std::byte *>(res.take_value());
    for (size_t i = 0; i < count; i++) {
      T *temp = reinterpret_cast<T *>(ptr + (i * sizeof(T)));
      std::construct_at(temp, std::forward<Args>(args)...);
    }
    return {std::move(reinterpret_cast<T *>(ptr))};
  }
  default: {
    return {error_enum::error};
  }
  }
}

template <typename A, typename T>
  requires is_allocator<A>
void delloc_type_array(A &allocator, typename A::t_handle handle,
                       size_t count) {
  if (count == 0) {
    crash_program(error_enum::alloc_array_bad_size,
                  "trying to free an array of size "
                  "zero");
  }

  switch (allocator.type_of()) {
  case allocator_type::raw: {
    if (!handle) {
      crash_program(error_enum::is_null_pointer, "trying to dealloc a nulptr");
    }
    const size_t al_size = aligned_size_type<T>();
    std::byte *temp_ptr = reinterpret_cast<std::byte *>(handle);
    for (size_t i = 0; i < count; i++) {
      T *temp = reinterpret_cast<T *>(temp_ptr + (i * al_size));
      temp->~T();
    }
    dynamic_cast<allocator_raw>(allocator).dealloc(handle);
  }
  default: {
    crash_program("ERROR");
    break;
  }
  }
}

} // namespace _details

} // namespace allocators
} // namespace core
} // namespace roco
