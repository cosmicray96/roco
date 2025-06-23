#include "roco_core/algo/sort.hpp"
#include "roco_core/allocators/heap.hpp"
#include "roco_core/collections/array.hpp"
#include "roco_core/collections/collection.hpp"
#include "roco_core/collections/iterator.hpp"

#include <cassert>

using namespace roco::core::allocators;
using namespace roco::core::collections;
using namespace roco::core::algo;
using namespace roco::core;
using heap = roco::core::allocators::heap;

class atest {
public:
  atest() = default;
  ~atest() = default;
  atest(int32_t i) : m_i(i) {}

  atest(const atest &other) = delete;
  atest &operator=(const atest &other) = delete;

  atest(atest &&other) = default;
  atest &operator=(atest &&other) = default;

  bool operator==(const atest &other) const { return m_i == other.m_i; }
  bool operator>(const atest &other) const { return m_i > other.m_i; }
  bool operator<(const atest &other) const { return m_i < other.m_i; }

  friend void swap(atest &a, atest &b) {
    using std::swap;
    swap(a.m_i, b.m_i);
  }

  friend std::ostream &operator<<(std::ostream &os, const atest &a) {
    os << "m_i(" << a.m_i << ")";
    return os;
  }

private:
  int32_t m_i = 0;
};

template <typename T>
  requires is_sortable<T> && is_collection_elem<T>
void randomize(array<T, 10, heap> &arr) {
  using std::swap;

  swap(arr[6], arr[9]);
  swap(arr[5], arr[4]);
  swap(arr[2], arr[7]);
  swap(arr[1], arr[0]);
  swap(arr[8], arr[3]);
}

int main() {

  static_assert(is_iterator<array_it<int32_t, 10, heap, false>>);
  static_assert(is_iterator<array_it<int32_t, 10, heap, true>>);
  static_assert(is_sortable<int32_t>);
  static_assert(is_collection_elem<int32_t>);
  static_assert(is_sortable<atest>);
  static_assert(is_collection_elem<atest>);

  { // basic creation
    auto res = array<int32_t, 10, heap>::make();
    assert(!res.has_error());
    array<int32_t, 10, heap> arr = res.take_value();
    for (size_t i = 0; i < 10; i++) {
      arr[i] = i;
    }

    array_it<int32_t, 10, heap, false> it1 = arr.to_array_it_beg();
    array_it<int32_t, 10, heap, false> it2 = arr.to_array_it_end();

    std::cout << "arr : " << arr << std::endl;

    randomize(arr);
    std::cout << "rand: " << arr << std::endl;
    sort(it1, it2);
    std::cout << "sort: " << arr << std::endl;

    for (size_t i = 0; it1 != it2; i++, it1.inc()) {
      assert(*it1 == i);
    }
  }
  { // basic creation
    auto res = array<atest, 10, heap>::make();
    assert(!res.has_error());
    array<atest, 10, heap> arr = res.take_value();
    for (size_t i = 0; i < 10; i++) {
      arr[i] = i;
    }

    array_it<atest, 10, heap, false> it1 = arr.to_array_it_beg();
    array_it<atest, 10, heap, false> it2 = arr.to_array_it_end();

    std::cout << "arr : " << arr << std::endl;

    randomize<atest>(arr);
    std::cout << "rand: " << arr << std::endl;
    sort(it1, it2);
    std::cout << "sort: " << arr << std::endl;

    for (size_t i = 0; it1 != it2; i++, it1.inc()) {
      assert(*it1 == i);
    }
  }
  return 0;
}
