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

template <typename t_elem> class collection {
  public:
    collection() = default;
    virtual ~collection() = default;
    collection(const collection &other) = default;
    collection &operator=(const collection &other) = default;
    collection &operator=(collection &&other) = default;
    collection(collection &&other) = default;

  public:
    virtual size_t count() = 0;
    virtual uptr<iterator<t_elem>> beg(allocators::allocator &a) = 0;
    virtual uptr<iterator<t_elem>> end(allocators::allocator &a) = 0;
};

} // namespace collections
} // namespace core
} // namespace roco
