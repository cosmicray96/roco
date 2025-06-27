#include "roco_core/algo/algo.hpp"
#include "roco_core/allocators/heap.hpp"
#include "roco_core/collections/collection.hpp"
#include "roco_core/collections/vector.hpp"
#include "roco_core/roco_core.hpp"
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

  static_assert(is_collection<vector<int32_t, heap>>);

  { // basic creation
    auto res = vector<int32_t, heap>::make();
    assert(!res.has_error());
    vector<int32_t, heap> v = res.take_value();
    assert(v.capacity() == vector_d::start_cap);
    assert(v.count() == 0);
    for (size_t i = 0; i < v.capacity(); i++) {
      assert(v[i] == 0);
    }
    assert(v.capacity() == vector_d::start_cap);
    assert(v.count() == v.capacity());
  }
  { // push
    auto v = vector<int32_t, heap>::make().take_value();
    for (size_t i = 0; i < algo::min(vector_d::start_cap - 1, 0); i++) {
      v.push(i);
    }

    for (size_t i = 0; i < algo::min(vector_d::start_cap - 1, 0); i++) {
      assert(v[i] == i);
    }
  }
  { // no resize untill full capacity
    auto v = vector<int32_t, heap>::make().take_value();
    size_t v_cap = v.capacity();
    for (size_t i = 0; i < v_cap; i++) {
      v.push(i);
    }
    DEBUG_VAR(v.capacity());
    DEBUG_VAR(v_cap);
    assert(v.capacity() == v_cap);
    assert(v.capacity() == v.count());
  }
  { // resize
    auto v = vector<int32_t, heap>::make().take_value();
    size_t v_cap = v.capacity();
    for (size_t i = 0; i < v_cap + 1; i++) {
      v.push(i);
    }
    assert(v.capacity() != v_cap);
    assert(v.count() == v_cap + 1);
    assert(v.capacity() == v_cap * vector_d::growth_factor);
  }
  { // move
    auto v = vector<int32_t, heap>::make().take_value();
    for (size_t i = 0; i < v.capacity(); i++) {
      v.push(i);
    }
    vector<int32_t, heap> v1 = std::move(v);
    assert(v.capacity() == 0);
    assert(v.count() == 0);
    assert(v1.capacity() == vector_d::start_cap);
    assert(v1.count() == v1.capacity());
  }
  { // move, ctor
    auto v = vector<int32_t, heap>::make().take_value();
    for (size_t i = 0; i < v.capacity(); i++) {
      v.push(i);
    }
    vector<int32_t, heap> v1(std::move(v));
    assert(v.capacity() == 0);
    assert(v.count() == 0);
    assert(v1.capacity() == vector_d::start_cap);
    assert(v1.count() == v1.capacity());
  }
  { // move, old deleted
    auto v = vector<int32_t, heap>::make().take_value();
    {
      auto v1 = vector<int32_t, heap>::make().take_value();
      for (size_t i = 0; i < v1.capacity(); i++) {
        v1.push(21);
      }
      v = std::move(v1);
    }
    for (size_t i = 0; i < v.capacity(); i++) {
      assert(v[i] == 21);
    }
  }
  { // move, resize
    auto v = vector<int32_t, heap>::make().take_value();
    size_t v_cap = v.capacity();
    for (size_t i = 0; i < v_cap + 1; i++) {
      v.push(i);
    }
    vector<int32_t, heap> v1 = std::move(v);
    for (size_t i = 0; i < v_cap + 1; i++) {
      assert(v1[i] == i);
    }
  }
  { // move, resize, ctor
    auto v = vector<int32_t, heap>::make().take_value();
    size_t v_cap = v.capacity();
    for (size_t i = 0; i < v_cap + 1; i++) {
      v.push(i);
    }
    vector<int32_t, heap> v1(std::move(v));
    for (size_t i = 0; i < v_cap + 1; i++) {
      assert(v1[i] == i);
    }
  }
  { // move, old deleted, resize
    auto v = vector<int32_t, heap>::make().take_value();
    size_t v_cap = v.capacity();
    {
      auto v1 = vector<int32_t, heap>::make().take_value();
      for (size_t i = 0; i < v_cap + 1; i++) {
        v1.push(21);
      }
      v = std::move(v1);
    }
    for (size_t i = 0; i < v_cap + 1; i++) {
      assert(v[i] == 21);
    }
  }
  {
    auto v = vector<int32_t, heap>::make().take_value();
    size_t index = algo::min(0, vector_d::start_cap - 1);
    v[index] = 10;
    assert(v.count() == index + 1);
  }

  return 0;
}
