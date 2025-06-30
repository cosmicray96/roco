#pragma once

#include "roco_core/allocators/allocator.hpp"
#include "roco_core/error.hpp"
#include "roco_core/error_enum.hpp"

namespace roco {
namespace core {
namespace allocators {

class heap : public allocator_raw {
public:
  heap() : allocator_raw() {}
  virtual ~heap() = default;

public:
  virtual result<void *, error_enum>
  alloc(size_t size, size_t align) override;
  virtual void dealloc(void *ptr) override;
};

} // namespace allocators
} // namespace core
} // namespace roco
