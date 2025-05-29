#pragma once

#include <concepts>
#include <cstddef>

namespace roco {
namespace dsa {
namespace collection {

template <typename T>
concept is_collection_elem =
    std::copyable<T> && std::movable<T> && std::default_initializable<T>;

template <typename T>
concept is_collection = requires(T a) {
  { a.count() } -> std::convertible_to<size_t>;
  T::t_elem_type;
} && is_collection_elem<typename T::t_elem>;
} // namespace collection
} // namespace dsa
} // namespace roco
