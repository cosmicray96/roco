#include "roco_core/allocators/allocator.hpp"
#include "roco_core/allocators/heap.hpp"
#include "roco_core/collections/array.hpp"
#include <vector>

namespace roco {
namespace core {
namespace allocators {

std::vector<allocator *> s_allocators = {&allocators::heap::s_};

void registry::init() {}
void registry::add(allocator *a) { s_allocators.push_back(a); }
void registry::dealloc(void *ptr) {
    for (size_t i = 0; i < s_allocators.size(); i++) {
        if (s_allocators[i]->belongs(ptr)) {
            s_allocators[i]->dealloc(ptr);
        }
    }
}

} // namespace allocators
} // namespace core
} // namespace roco
