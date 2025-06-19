#include "roco_core/allocators/heap.hpp"
#include "roco_core/allocators/allocator.hpp"
#include "roco_core/error_enum.hpp"
#include "roco_core/roco_core.hpp"
#include <cstdlib>

namespace roco {
namespace core {
namespace allocators {

roco::core::result<void *, roco::core::error_enum> heap::alloc(size_t size, size_t align) {
    using namespace roco::core;

    if (size == 0) {
        return {error_enum::alloc_bad_size};
    }
    if (!is_non_zero_pow_2(align)) {
        return {error_enum::alloc_bad_align};
    }

    void *ptr = aligned_alloc(align, aligned_size(size, align));
    if (!ptr) {
        return {error_enum::alloc_bad_alloc};
    }
    return {std::move(ptr)};
}
roco::core::result<void *, roco::core::error_enum> heap::alloc8(size_t size) {
    return alloc(size, 8);
}
roco::core::result<void *, roco::core::error_enum> heap::alloc16(size_t size) {
    return alloc(size, 16);
}
optional<roco::core::error_enum> heap::dealloc(void *ptr) {
    if (!ptr) {
        return {roco::core::error_enum::is_null_pointer};
    }
    std::free(ptr);
    return {};
}

} // namespace allocators
} // namespace core
} // namespace roco
