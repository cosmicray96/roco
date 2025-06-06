

#include "roco_core/allocators/heap.hpp"
#include "roco_core/collections/array.hpp"
#include "roco_core/smart_ptr.hpp"
#include <cassert>
#include <cstdint>

using namespace roco::core::collections;
using namespace roco::core;
int main() {

    allocators::heap &heap = allocators::heap::s_;
    array<int> arr(10, heap);

    std::cout << "arr at 0: " << arr << std::endl;

    arr[0] = 10;
    std::cout << "arr at 1: " << arr << std::endl;
    arr[1] = 11;
    std::cout << "arr at 2: " << arr << std::endl;
    arr[2] = 12;
    std::cout << "arr at 3: " << arr << std::endl;

    std::cout << "count: " << arr.count() << std::endl;
    assert(arr.count() == 3);

    array<int32_t> arr1(10, heap);
    for (size_t i = 0; i < 5; i++) {
        arr1[i] = i * 10;
    }
    array_it<int32_t> it1 = to_array_it_beg<int32_t>(arr1);
    array_it<int32_t> it2 = to_array_it_end(arr1);

    for (size_t i = 0; !it1.equals(it2); it1.inc(), i++) {
        assert(it1.get() == i * 10);
    }

    uptr<iterator<int32_t>> it3 = into_iterator(to_array_it_beg(arr1), heap);
    uptr<iterator<int32_t>> it4 = into_iterator(to_array_it_end(arr1), heap);

    for (size_t i = 0; !it3->equals(*it4.get()); it3->inc(), i++) {
        assert(it3->get() == i * 10);
    }

    ///////

    array<int32_t> arr4(10, heap);
    for (size_t i = 0; i < 10; i++) {
        arr4[i] = i * 10;
    }
    array_it<int32_t> it7 = to_array_it_beg(arr4);
    array_it<int32_t> it8 = to_array_it_end(arr4);
    while (!it7.equals(it8)) {
        (*it7) = 5;
        it7.inc();
    }
    for (size_t i = 0; i < 10; i++) {
        assert(arr4[i] == 5);
    }

    ///////////

    array<int32_t> arr3(10, heap);
    for (size_t i = 0; i < 10; i++) {
        arr3[i] = i * 10;
    }
    uptr<collection_view<int32_t>> col3 = to_collection_view(arr3, heap);
    array_it<int32_t> it5 = to_array_it_beg(arr3);
    uptr<iterator<int32_t>> it6 = col3->beg(heap);
    for (size_t i = 0; i < 10; i++) {
        assert(it5.get() == it6->get());
    }
    for (size_t i = 0; i < 10; i++) {
    }

    //////

    array<int32_t> arr5(10, heap);
    for (size_t i = 0; i < 10; i++) {
        arr5[i] = i * 10;
    }
    array<int32_t> arr7(std::move(arr5));
    assert(arr5.count() == 0);
    assert(arr7.count() == 10);
    for (size_t i = 0; i < 10; i++) {
        assert(arr7[i] == i * 10);
    }

    array<int32_t> arr9 = array<int32_t>(10, heap);
    assert(arr9.capacity() == 10);

    return 0;
}
