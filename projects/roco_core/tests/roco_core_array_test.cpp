#include "roco_core/allocators/heap.hpp"
#include "roco_core/collections/array.hpp"
#include <cassert>

using namespace roco::core::collection;
int main() {
  array<int> arr(10, &roco::core::allocators::heap::s_);

  std::cout << "arr at 0: " << arr << std::endl;

  arr[0] = 10;
  std::cout << "arr at 1: " << arr << std::endl;
  arr[1] = 11;
  std::cout << "arr at 2: " << arr << std::endl;
  arr[2] = 12;
  std::cout << "arr at 3: " << arr << std::endl;

  std::cout << "count: " << arr.count() << std::endl;
  assert(arr.count() == 3);

  return 0;
}
