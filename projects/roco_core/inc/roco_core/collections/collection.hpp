#pragma once

#include <concepts>
#include <cstddef>
#include <memory>

#include "roco_core/collections/iterator.hpp"

namespace roco {
namespace core {
namespace collection {

template <typename T>
concept is_collection_elem = std::copyable<T> && std::movable<T> && std::default_initializable<T>;

template <typename t_elem> class collection {
  public:
    collection() = default;
    virtual ~collection() = default;

  public:
    virtual size_t count() = 0;
    virtual iterator<t_elem> beg() = 0;
    virtual iterator<t_elem> end() = 0;
};

} // namespace collection
} // namespace core
} // namespace roco
