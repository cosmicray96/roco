#pragma once

#include "roco_core/roco_core.hpp"
#include <concepts>
#include <cstddef>
#include <memory>

namespace roco {
namespace core {
namespace collections {

template <typename T>
concept is_collection_elem = std::copyable<T> && roco::core::is_movable<T> &&
                             std::default_initializable<T>;

} // namespace collections
} // namespace core
} // namespace roco
