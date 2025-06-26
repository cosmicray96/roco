#pragma once

#include "roco_core/error.hpp"
#include "roco_core/error_enum.hpp"
#include "roco_core/roco_core.hpp"
#include <concepts>
#include <cstddef>
#include <memory>

namespace roco {
namespace core {
namespace collections {

namespace _details {
template <typename T>
concept has_default_make = requires() {
  {
    T::make()
  } -> std::convertible_to<roco::core::result<T, roco::core::error_enum>>;
};
template <typename T>
concept has_copy_make = requires() {
  {
    T::make(typename T::t_elem())
  } -> std::convertible_to<roco::core::result<T, roco::core::error_enum>>;
};
} // namespace _details

template <typename T>
concept is_collection_elem =
    roco::core::is_copyable<T> && roco::core::is_movable<T> &&
    std::default_initializable<T>;

template <typename T>
concept is_collection =
    is_collection_elem<typename T::t_elem> &&
    (_details::has_default_make<T> || _details::has_copy_make<T>);

} // namespace collections
} // namespace core
} // namespace roco
