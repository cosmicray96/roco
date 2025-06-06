

#include "roco_core/allocators/heap.hpp"
#include "roco_core/collections/array.hpp"
#include "roco_core/smart_ptr.hpp"
#include <cassert>
#include <cstdint>

using namespace roco::core::collections;
using namespace roco::core;
int main() {
    using namespace roco::core::allocators;

    array<int, 10, heap> arr;

    std::cout << "arr at 0: " << arr << std::endl;

    arr[0] = 10;
    std::cout << "arr at 1: " << arr << std::endl;
    arr[1] = 11;
    std::cout << "arr at 2: " << arr << std::endl;
    arr[2] = 12;
    std::cout << "arr at 3: " << arr << std::endl;

    std::cout << "count: " << arr.count() << std::endl;
    assert(arr.count() == 3);

    array<int32_t, 10, heap> arr1;
    for (size_t i = 0; i < 5; i++) {
        arr1[i] = i * 10;
    }
    array_it<int32_t> it1 = arr1.to_array_it_beg();
    array_it<int32_t> it2 = arr1.to_array_it_end();

    for (size_t i = 0; !it1.equals(it2); it1.inc(), i++) {
        assert(it1.get() == i * 10);
    }

    uptr<iterator<int32_t>, heap> it3 = arr1.to_array_it_beg().to_iterator<heap>();

    uptr<iterator<int32_t>, heap> it4 =
        make_uptr_dyn<iterator<int32_t>, array_it<int32_t>, heap>(arr1.to_array_it_end());

    for (size_t i = 0; !it3->equals(*it4.get()); it3->inc(), i++) {
        assert(it3->get() == i * 10);
    }

    ///////

    array<int32_t, 10, heap> arr4;
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

    ///////////

    array<int32_t, 10, heap> arr3;
    for (size_t i = 0; i < 10; i++) {
        arr3[i] = i * 10;
    }

    //////

    array<int32_t, 10, heap> arr5;
    for (size_t i = 0; i < 10; i++) {
        arr5[i] = i * 10;
    }
    array<int32_t, 10, heap> arr7(std::move(arr5));
    assert(arr5.count() == 0);
    assert(arr7.count() == 10);
    for (size_t i = 0; i < 10; i++) {
        assert(arr7[i] == i * 10);
    }

    array<int32_t, 10, heap> arr9 = array<int32_t, 10, heap>();
    assert(arr9.capacity() == 10);

    return 0;
}
