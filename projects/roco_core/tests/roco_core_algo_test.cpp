#include "roco_core/algo/sort.hpp"
#include "roco_core/allocators/heap.hpp"
#include "roco_core/collections/array.hpp"
#include "roco_core/collections/collection.hpp"
#include "roco_core/collections/iterator.hpp"

#include <cassert>
#include <random>

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
  requires is_iterator_randacc<T>
void randomize(span<T> s) {
  using std::swap;
  T it1 = s.b;
  T it2 = s.e;
  it2.dec();
  if (it2 == it1) {
    return;
  }
  std::mt19937 rng(1000);
  std::uniform_int_distribution<int> dist(0, 1);

  while (it1 != s.e) {
    T it2 = s.e;
    it2.dec();
    while (it1 != it2) {
      if (dist(rng)) {
        swap(*it1, *it2);
      }

      it2.dec();
    }
    it1.inc();
  }
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

    span<array_it<int32_t, 10, heap, false>> s(arr.to_array_it_beg(),
                                               arr.to_array_it_end());
    randomize(s);
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
    span<array_it<atest, 10, heap, false>> s(arr.to_array_it_beg(),
                                             arr.to_array_it_end());
    randomize(s);
    std::cout << "rand: " << arr << std::endl;
    sort(it1, it2);
    std::cout << "sort: " << arr << std::endl;

    for (size_t i = 0; it1 != it2; i++, it1.inc()) {
      assert(*it1 == i);
    }
  }
  return 0;
}
