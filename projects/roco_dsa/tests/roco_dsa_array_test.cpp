#include "roco_dsa/allocators/heap.hpp"
#include "roco_dsa/collections/array.hpp"
#include <cassert>

using namespace roco::dsa::collection;
int main() {
  array<int, allocator_heap> arr(10);

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
