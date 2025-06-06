#pragma once

#include "roco_core/allocators/allocator.hpp"
#include <cstdlib>

namespace roco {
namespace core {
namespace allocators {

class heap {
  public:
    static void *alloc(size_t size, size_t align);
    static void *alloc8(size_t size);
    static void *alloc16(size_t size);
    static void dealloc(void *ptr);
};

} // namespace allocators
} // namespace core
} // namespace roco
