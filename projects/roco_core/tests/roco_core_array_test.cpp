#include "roco_core/allocators/heap.hpp"
#include "roco_core/collections/array.hpp"
#include "roco_core/smart_ptr.hpp"
#include <cassert>
#include <cstdint>

#include <concepts>
#include <type_traits>
#include <utility>

int main() {
  using namespace roco::core::allocators;
  using namespace roco::core::collections;
  using namespace roco::core;
  {
    auto res = array<int32_t, 10, heap>::make();
    if (res.has_error()) {
      return 1;
    }
    array<int32_t, 10, heap> arr = res.take_value();

    std::cout << "arr at 0: " << arr << std::endl;

    arr[0] = 10;
    std::cout << "arr at 1: " << arr << std::endl;
    arr[1] = 11;
    std::cout << "arr at 2: " << arr << std::endl;
    arr[2] = 12;
    std::cout << "arr at 3: " << arr << std::endl;

    std::cout << "count: " << arr.count() << std::endl;
    assert(arr.count() == 3);
  }
  {
    auto res1 = array<int32_t, 10, heap>::make();
    if (res1.has_error()) {
      return 1;
    }
    array<int32_t, 10, heap> arr1 = res1.take_value();
    for (size_t i = 0; i < 5; i++) {
      arr1[i] = i * 10;
    }
    array_it<int32_t> it1 = arr1.to_array_it_beg();
    array_it<int32_t> it2 = arr1.to_array_it_end();

    for (size_t i = 0; !it1.equals(it2); it1.inc(), i++) {
      assert(it1.get() == i * 10);
    }

    auto it3_res = arr1.to_array_it_beg().to_iterator<heap>();
    if (it3_res.has_error()) {
      return 1;
    }
    uptr<iterator<int32_t>, heap> it3 = it3_res.take_value();

    auto it4_res = arr1.to_array_it_end().to_iterator<heap>();
    if (it4_res.has_error()) {
      return 1;
    }
    uptr<iterator<int32_t>, heap> it4 = it4_res.take_value();

    for (size_t i = 0; !it3->equals(*it4.get()); it3->inc(), i++) {
      assert(it3->get() == i * 10);
    }
  }
  ///////
  {
    auto res4 = array<int32_t, 10, heap>::make();
    if (res4.has_error()) {
      return 1;
    }
    array<int32_t, 10, heap> arr4 = res4.take_value();
    for (size_t i = 0; i < 10; i++) {
      arr4[i] = i * 10;
    }
    array_it<int32_t> it7 = arr4.to_array_it_beg();
    array_it<int32_t> it8 = arr4.to_array_it_end();
    while (!it7.equals(it8)) {
      (*it7) = 5;
      it7.inc();
    }
    for (size_t i = 0; i < 10; i++) {
      assert(arr4[i] == 5);
    }
  }
  ///////////
  {
    auto res3 = array<int32_t, 10, heap>::make();
    if (res3.has_error()) {
      return 1;
    }
    array<int32_t, 10, heap> arr3 = res3.take_value();
    for (size_t i = 0; i < 10; i++) {
      arr3[i] = i * 10;
    }
  }
  //////
  {
    auto res5 = array<int32_t, 10, heap>::make();
    if (res5.has_error()) {
      return 1;
    }
    array<int32_t, 10, heap> arr5 = res5.take_value();
    for (size_t i = 0; i < 10; i++) {
      arr5[i] = i * 10;
    }
    array<int32_t, 10, heap> arr7(std::move(arr5));
    assert(arr5.count() == 0);
    assert(arr7.count() == 10);
    for (size_t i = 0; i < 10; i++) {
      assert(arr7[i] == i * 10);
    }
  }
  {
    auto res9 = array<int32_t, 10, heap>::make();
    if (res9.has_error()) {
      return 1;
    }
    array<int32_t, 10, heap> arr9 = res9.take_value();
    assert(arr9.capacity() == 10);

    return 0;
  }
}
