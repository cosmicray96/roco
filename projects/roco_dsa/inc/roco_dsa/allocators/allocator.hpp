#pragma once

#include <concepts>
#include <cstddef>

namespace roco {
namespace dsa {
namespace collection {

template <typename T>
concept is_allocator = requires {
  { T::alloc(std::size_t{}) } -> std::convertible_to<void *>;
  { T::dealloc((void *)nullptr) } -> std::same_as<void>;
};

} // namespace collection
} // namespace dsa
} // namespace roco
