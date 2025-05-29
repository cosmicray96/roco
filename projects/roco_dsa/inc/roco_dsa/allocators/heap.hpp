#pragma once

#include "roco_dsa/allocators/allocator.hpp"
#include <cstdlib>

namespace roco {
namespace dsa {
namespace collection {

class allocator_heap {
public:
  static void *alloc(size_t byte_count) { return malloc(byte_count); }
  static void dealloc(void *ptr) { free(ptr); }
};

} // namespace collection
} // namespace dsa
} // namespace roco
