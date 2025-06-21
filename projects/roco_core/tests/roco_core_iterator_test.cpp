#include "roco_core/allocators/heap.hpp"
#include "roco_core/collections/array.hpp"
#include "roco_core/collections/iterator.hpp"

#include <cassert>
#include <iostream>

template <typename T>
  requires roco::core::collections::
      is_iterator<T>
    T::t_ref iterator_func(T &it) {
  return it.get();
}

template <typename T>
  requires roco::core::collections::
      is_iterator_back<T>
    T::t_ref iterator_back_func(T &it) {
  it.dec();
  return it.get();
}

template <typename T>
  requires roco::core::collections::
      is_iterator_randacc<T>
    T::t_ref iterator_randacc_func(T &it) {
  it.inc(2);
  return it.get();
}

int main() {
  using namespace roco::core::allocators;
  using namespace roco::core::collections;
  using namespace roco::core;
  using heap = roco::core::allocators::heap;

  static_assert(
      is_iterator<array_it<int32_t, false>>);
  static_assert(
      is_iterator<array_it<int32_t, true>>);

  { // basic creation
    auto res =
        array<int32_t, 10, heap>::make();
    assert(!res.has_error());
    array<int32_t, 10, heap> arr =
        res.take_value();
    for (size_t i = 0; i < 10; i++) {
      arr[i] = i;
    }

    array_it<int32_t, false> it1 =
        arr.to_array_it_beg();
    array_it<int32_t, false> it2 =
        arr.to_array_it_end();
    assert(it1.ptr() != nullptr);
    assert(it2.ptr() != nullptr);

    for (size_t i = 0; it1 != it2;
         i++, it1.inc()) {
      assert(*it1 == i);
    }
  }
  { // iterator concepts
    auto res =
        array<int32_t, 10, heap>::make();
    assert(!res.has_error());
    array<int32_t, 10, heap> arr =
        res.take_value();
    for (size_t i = 0; i < 10; i++) {
      arr[i] = i;
    }

    array_it<int32_t, false> it1 =
        arr.to_array_it_beg();
    array_it<int32_t, false> it2 =
        arr.to_array_it_end();

    it1.inc();

    int32_t &value = iterator_func<
        array_it<int32_t, false>>(it1);
    assert(value == 1);

    int32_t &value1 = iterator_back_func<
        array_it<int32_t, false>>(it1);
    assert(value1 == 0);

    int32_t &value2 = iterator_randacc_func<
        array_it<int32_t, false>>(it1);
    assert(value2 == 2);
  }
  { // const

    auto res =
        array<int32_t, 10, heap>::make();
    assert(!res.has_error());
    array<int32_t, 10, heap> arr =
        res.take_value();
    for (size_t i = 0; i < 10; i++) {
      arr[i] = i;
    }

    array_it<int32_t, true> it1 =
        arr.to_array_it_beg_const();
    array_it<int32_t, true> it2 =
        arr.to_array_it_end_const();

    for (size_t i = 0; it1 != it2;
         i++, it1.inc()) {
      assert(*it1 == i);
    }

    array_it<int32_t, true> it3 =
        arr.to_array_it_beg_const();
    array_it<int32_t, true> it4 =
        arr.to_array_it_end_const();

    // should fail
    // *it3 = 10;
    // *(it3.ptr()) = 10;
  }
  { // const
    auto res =
        array<int32_t, 10, heap>::make(21);
    assert(!res.has_error());
    const array<int32_t, 10, heap> arr =
        res.take_value();

    array_it<int32_t, true> it1 =
        arr.to_array_it_beg_const();
    array_it<int32_t, true> it2 =
        arr.to_array_it_end_const();

    for (; it1 != it2; it1.inc()) {
      assert(*it1 == 21);
    }
  }

  return 0;
}
