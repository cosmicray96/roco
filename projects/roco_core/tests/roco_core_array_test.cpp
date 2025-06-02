#include "roco_core/allocators/heap.hpp"
#include "roco_core/collections/array.hpp"
#include <cassert>
#include <cstdint>

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

    array<int32_t> arr1(10, &roco::core::allocators::heap::s_);
    for (size_t i = 0; i < 5; i++) {
        arr1[i] = i * 10;
    }
    array_iterator<int32_t> it1 = arr1.beg();
    array_iterator<int32_t> it2 = arr1.end();

    for (size_t i = 0; !it1.equals(it2); it1.inc(), i++) {
        assert(it1.get() == i * 10);
    }

    std::unique_ptr<iterator<int32_t>> it3 = arr1.beg().to_iterator();
    std::unique_ptr<iterator<int32_t>> it4 = arr1.beg().to_iterator();

    for (size_t i = 0; !it3->equals(*it4.get()); it3->inc(), i++) {
        assert(it3->get() == i * 10);
    }

    return 0;
}
