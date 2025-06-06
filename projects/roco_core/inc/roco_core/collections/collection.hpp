#pragma once

#include <concepts>
#include <cstddef>
#include <memory>

#include "roco_core/allocators/allocator.hpp"
#include "roco_core/collections/iterator.hpp"
#include "roco_core/smart_ptr.hpp"

namespace roco {
namespace core {
namespace collections {

template <typename T>
concept is_collection_elem = std::copyable<T> && std::movable<T> && std::default_initializable<T>;

} // namespace collections
} // namespace core
} // namespace roco
