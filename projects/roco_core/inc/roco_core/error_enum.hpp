#pragma once

namespace roco {
namespace core {
enum class error_enum {
    success,
    error,

    is_null_pointer,

    alloc_bad_alloc,
    alloc_bad_dealloc,
    alloc_bad_align,
    alloc_bad_size,
    alloc_out_of_memory,
    alloc_array_bad_size,
};
}
} // namespace roco
