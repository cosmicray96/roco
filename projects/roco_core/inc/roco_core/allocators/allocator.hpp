#pragma once

#include <concepts>
#include <cstddef>

namespace roco {
namespace core {
namespace allocators {

class allocator {
  public:
    virtual void *alloc(size_t bytes) = 0;
    virtual void dealloc(void *ptr) = 0;
    virtual bool belongs(void *ptr) = 0;
};

class registry {
  private:
    registry();

  public:
    static void init();
    static void add(allocator *a);
    static void dealloc(void *ptr);
};

} // namespace allocators
} // namespace core
} // namespace roco
