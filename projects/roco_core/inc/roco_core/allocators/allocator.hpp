#pragma once

#include <concepts>
#include <cstddef>
#include <utility>

namespace roco {
namespace core {
namespace allocators {

template <typename A>
concept is_allocator = requires(A a, size_t size, size_t align, void *ptr) {
    { A::alloc(size, align) } -> std::same_as<void *>;
    { A::alloc8(size) } -> std::same_as<void *>;
    { A::alloc16(size) } -> std::same_as<void *>;
    { A::dealloc(ptr) } -> std::same_as<void>;
};

constexpr size_t aligned_size(size_t size, size_t align) {
    return ((size + align - 1) / align) * align;
}
template <typename T> constexpr size_t aligned_size() {
    constexpr size_t size = sizeof(T);
    constexpr size_t align = alignof(T);
    return aligned_size(size, align);
}

template <typename A, typename T, typename... Args>
    requires is_allocator<A>
T *alloc_type(Args &&...args) {
    void *ptr = A::alloc(sizeof(T), alignof(T));
    T *output = new (ptr) T(std::forward(args)...);
    return output;
}
template <typename A, typename T>
    requires is_allocator<A>
void dealloc_type(T *ptr) {
    ptr->~T();
    A::dealloc(ptr);
}

} // namespace allocators
} // namespace core
} // namespace roco
