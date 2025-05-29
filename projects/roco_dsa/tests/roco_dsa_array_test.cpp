#include "roco_dsa/allocators/heap.hpp"
#include "roco_dsa/collections/array.hpp"

using namespace roco::dsa::collection;
int main() {
  array<int, allocator_heap> arr(10);
  return 0;
}
