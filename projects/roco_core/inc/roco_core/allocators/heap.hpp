#pragma once

#include "roco_core/allocators/allocator.hpp"
#include <cstdlib>

namespace roco {
namespace core {
namespace collection {

class allocator_heap {
  public:
    static void *alloc(size_t byte_count) { return malloc(byte_count); }
    static void dealloc(void *ptr) { free(ptr); }
};

} // namespace collection
} // namespace core
} // namespace roco
