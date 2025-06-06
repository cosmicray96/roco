#include "roco_core/allocators/heap.hpp"
#include "roco_core/allocators/allocator.hpp"
#include "roco_core/roco_core.hpp"
#include <cstdlib>

namespace roco {
namespace core {
namespace allocators {

void *heap::alloc(size_t size, size_t align) {
    using namespace roco_core;
    void *ptr = aligned_alloc(align, aligned_size(size, align));
    if (!ptr) {
        crash_program("ptr null on allocation");
    }
    return ptr;
}
void *heap::alloc8(size_t size) { return alloc(size, 8); }
void *heap::alloc16(size_t size) { return alloc(size, 16); }
void heap::dealloc(void *ptr) { std::free(ptr); }

} // namespace allocators
} // namespace core
} // namespace roco
