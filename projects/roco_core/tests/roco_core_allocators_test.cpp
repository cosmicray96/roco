#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <limits>

#include "roco_core/allocators/allocator.hpp"
#include "roco_core/allocators/heap.hpp"

size_t s_ctor_counter = 0;
size_t s_dtor_counter = 0;
size_t s_move_counter = 0;
void reset_counters() {

  s_ctor_counter = 0;
  s_dtor_counter = 0;
  s_move_counter = 0;
}
void print_counters() {
  std::cout << "ctor: " << s_ctor_counter << ", dtor: " << s_dtor_counter
            << ", move: " << s_move_counter << std::endl;
}
class atest {
public:
  atest() { s_ctor_counter++; }
  ~atest() { s_dtor_counter++; }
  atest(atest &&other) { s_move_counter++; }
  atest &operator=(atest &&other) {
    s_move_counter++;
    return *this;
  }
  atest(const atest &other) = delete;
  atest &operator=(const atest &other) = delete;
};

int main() {

  using namespace roco::core;
  using namespace roco::core::allocators;
  using A = roco::core::allocators::heap;

  { // basic raw ptr
    auto res = A::alloc(64, 8);
    assert(!res.has_error());
    void *p = res.take_value();
    uint64_t data = std::numeric_limits<uint64_t>::max();
    memcpy(p, reinterpret_cast<void *>(&data), 64);
    assert(*reinterpret_cast<uint64_t *>(p) == data);
    std::cout << "p: " << *reinterpret_cast<uint64_t *>(p) << std::endl;

    auto op = A::dealloc(p);
    assert(!op.has_value());
  }
  { // array
    auto res = alloc_type_array<A, int32_t>(10, 5);
    assert(!res.has_error());
    int32_t *p = res.take_value();
    for (size_t i = 0; i < 10; i++) {
      assert(p[i] == 5);
    }
    for (size_t i = 0; i < 10; i++) {
      p[i] = 15;
    }
    for (size_t i = 0; i < 10; i++) {
      assert(p[i] == 15);
    }

    delloc_type_array<A>(p, 10);
  }
  { // alloc

    auto res = A::alloc(0, 16);
    assert(res.has_error());
    assert(res.take_error() == error_enum::alloc_bad_size);
  }
  { // alignment

    auto res = A::alloc(32, 32);
    assert(!res.has_error());
    void *p = res.take_value();
    assert(reinterpret_cast<uintptr_t>(p) % 32 == 0);
    auto op = A::dealloc(p);
    assert(!op.has_value());
  }
  { // alignment

    auto res = A::alloc8(32);
    assert(!res.has_error());
    void *p = res.take_value();
    assert(reinterpret_cast<uintptr_t>(p) % 8 == 0);
    auto op = A::dealloc(p);
    assert(!op.has_value());
  }
  { // alignment

    auto res = A::alloc16(32);
    assert(!res.has_error());
    void *p = res.take_value();
    assert(reinterpret_cast<uintptr_t>(p) % 16 == 0);
    auto op = A::dealloc(p);
    assert(!op.has_value());
  }
  { // alignment
    auto res = A::alloc(32, 5);
    assert(res.has_error());
    assert(res.take_error() == error_enum::alloc_bad_align);
  }
  { // dealloc
    auto op = A::dealloc(nullptr);
    assert(op.has_value());
    assert(op.take_value() == error_enum::is_null_pointer);
  }
  { // array
    auto res = alloc_type_array<A, int32_t>(0, 10);
    assert(res.has_error());
    assert(res.take_error() == error_enum::alloc_array_bad_size);
  }
  { // array

    reset_counters();
    auto res = alloc_type_array<A, atest>(10);
    assert(!res.has_error());
    atest *p = res.take_value();
    assert(s_ctor_counter == 10);

    delloc_type_array<A>(p, 10);
    assert(s_dtor_counter == 10);
  }
  { // stress
    for (size_t i = 0; i < 1000; ++i) {
      auto res = alloc_type<heap, int32_t>(i);
      assert(!res.has_error());
      dealloc_type<heap>(res.take_value());
    }
  }

  return 0;
}
