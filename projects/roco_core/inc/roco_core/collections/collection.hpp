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

template <typename t_elem, typename A>
    requires allocators::is_allocator<A>
class collection_view {
  public:
    collection_view() = default;
    virtual ~collection_view() = default;
    collection_view(const collection_view &other) = default;
    collection_view &operator=(const collection_view &other) = default;
    collection_view &operator=(collection_view &&other) = default;
    collection_view(collection_view &&other) = default;

  public:
    virtual size_t count() = 0;
    virtual uptr<iterator<t_elem>, A> beg() = 0;
    virtual uptr<iterator<t_elem>, A> end() = 0;
};

} // namespace collections
} // namespace core
} // namespace roco
