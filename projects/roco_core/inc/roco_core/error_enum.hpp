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
  alloc_out_of_memory,

};
}
} // namespace roco
