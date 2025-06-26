#include "roco_core/allocators/heap.hpp"
#include "roco_core/collections/array.hpp"
#include "roco_core/smart_ptr.hpp"
#include <cassert>
#include <cstdint>

#include <concepts>
#include <type_traits>
#include <utility>

class atest {
public:
  atest() : m_i(0) {}
  atest(int32_t i) : m_i(i) {}
  ~atest() = default;

  atest(const atest &other) = default;
  atest &operator=(const atest &other) = default;

  atest(atest &&other) = default;
  atest &operator=(atest &&other) = default;

public:
  int32_t get() { return m_i; }

  friend std::ostream &operator<<(std::ostream &os, atest &a) {
    os << a.m_i;
    return os;
  }

private:
  int32_t m_i;
};

int main() {
  using namespace roco::core::allocators;
  using namespace roco::core::collections;
  using namespace roco::core;
  using heap = roco::core::allocators::heap;

  { // array creation success, default ctor
    auto res = array<int32_t, 10, heap>::make();
    assert(!res.has_error());
    array<int32_t, 10, heap> arr = res.take_value();
    assert(arr.capacity() == 10);
  }
  { // ctor with value
    auto res = array<int32_t, 10, heap>::make(21);
    assert(!res.has_error());
    array<int32_t, 10, heap> arr = res.take_value();
    for (size_t i = 0; i < arr.capacity(); i++) {
      assert(arr[i] == 21);
    }
  }
  { // non default ctor
    auto res = array<atest, 10, heap>::make(21);
    assert(!res.has_error());
    array<atest, 10, heap> arr = res.take_value();
    for (size_t i = 0; i < arr.capacity(); i++) {
      assert(arr[i].get() == 21);
    }
  }
  { // non default with no args
    //    auto res = array<atest, 10,
    //    heap>::make();
  }
  { // moves assign
    auto res = array<int32_t, 10, heap>::make(21);
    assert(!res.has_error());
    array<int32_t, 10, heap> arr = res.take_value();
    array<int32_t, 10, heap> arr1 = std::move(arr);
    for (size_t i = 0; i < arr1.capacity(); i++) {
      assert(arr1[i] == 21);
    }
  }
  { // move ctor
    auto res = array<int32_t, 10, heap>::make(21);
    assert(!res.has_error());
    array<int32_t, 10, heap> arr = res.take_value();
    array<int32_t, 10, heap> arr1(std::move(arr));
    for (size_t i = 0; i < arr1.capacity(); i++) {
      assert(arr1[i] == 21);
    }
  }
  { // move assign into existing
    auto res = array<int32_t, 10, heap>::make(21);
    assert(!res.has_error());
    array<int32_t, 10, heap> arr = res.take_value();
    auto res1 = array<int32_t, 10, heap>::make(42);
    assert(!res1.has_error());
    array<int32_t, 10, heap> arr1 = res1.take_value();
    arr1 = std::move(arr);
    for (size_t i = 0; i < arr1.capacity(); i++) {
      assert(arr1[i] == 21);
    }
  }
  { // moved into another then died
    auto res = array<int32_t, 10, heap>::make(21);
    assert(!res.has_error());
    array<int32_t, 10, heap> arr = res.take_value();
    {
      auto res1 = array<int32_t, 10, heap>::make(42);
      assert(!res1.has_error());
      array<int32_t, 10, heap> arr1 = res1.take_value();
      arr = std::move(arr1);
    }
    for (size_t i = 0; i < arr.capacity(); i++) {
      assert(arr[i] == 42);
    }
  }
}
