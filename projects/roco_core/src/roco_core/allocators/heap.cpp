#include "roco_core/allocators/heap.hpp"
#include "roco_core/allocators/allocator.hpp"
#include "roco_core/error_enum.hpp"
#include "roco_core/roco_core.hpp"
#include <cstdlib>

namespace roco {
namespace core {
namespace allocators {

result<void *, error_enum>
heap::alloc(size_t size, size_t align) {

  if (size == 0) {
    return {error_enum::alloc_bad_size};
  }
  if (!is_non_zero_pow_2(align)) {
    return {error_enum::alloc_bad_align};
  }

  void *ptr = aligned_alloc(
      align, aligned_size(size, align));
  if (!ptr) {
    return {error_enum::alloc_bad_alloc};
  }
  return {std::move(ptr)};
}
void heap::dealloc(void *ptr) {
  if (!ptr) {
    crash_program(error_enum::error,
                  "trying to free nulptr");
  }
  std::free(ptr);
}

} // namespace allocators
} // namespace core
} // namespace roco
