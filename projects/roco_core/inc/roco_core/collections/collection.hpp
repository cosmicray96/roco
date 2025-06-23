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

template <typename T>
concept is_collection_elem =
    roco::core::is_movable<T> && std::default_initializable<T>;

template <typename T>
concept is_collection =
    is_collection_elem<typename T::t_elem> &&
    (requires(T t) {
      {
        T::make()
      } -> std::convertible_to<
            roco::core::result<
                T, roco::core::error_enum>>;
    }
||
requires(T t) {
      {
        T::make(T::t_elem())
      } -> std::convertible_to<
            roco::core::result<
                T, roco::core::error_enum>>;
})
;

} // namespace collections
} // namespace core
} // namespace roco
