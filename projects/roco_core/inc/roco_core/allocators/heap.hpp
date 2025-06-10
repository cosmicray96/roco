#pragma once

#include "roco_core/allocators/allocator.hpp"
#include "roco_core/error.hpp"
#include "roco_core/error_enum.hpp"

namespace roco {
namespace core {
namespace allocators {

class heap {
public:
  static roco::core::result<void *, roco::core::error_enum> alloc(size_t size,
                                                                  size_t align);
  static roco::core::result<void *, roco::core::error_enum> alloc8(size_t size);
  static roco::core::result<void *, roco::core::error_enum>
  alloc16(size_t size);
  static optional<roco::core::error_enum> dealloc(void *ptr);
};

} // namespace allocators
} // namespace core
} // namespace roco
