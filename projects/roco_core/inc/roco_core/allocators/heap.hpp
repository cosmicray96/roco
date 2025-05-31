#pragma once

#include "roco_core/allocators/allocator.hpp"
#include <cstdlib>

namespace roco {
namespace core {
namespace allocators {

class heap : public allocator {
public:
  virtual void *alloc(size_t byte_count) override { return malloc(byte_count); }
  virtual void dealloc(void *ptr) override { free(ptr); }
  virtual bool belongs(void *ptr) override { return true; }

public:
  static heap s_;
};

} // namespace allocators
} // namespace core
} // namespace roco
