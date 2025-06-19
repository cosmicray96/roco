#include "roco_core/allocators/allocator.hpp"
#include "roco_core/allocators/heap.hpp"
#include "roco_core/smart_ptr.hpp"
#include <cassert>
#include <iostream>

size_t s_ctor_counter = 0;
size_t s_dtor_counter = 0;
size_t s_move_counter = 0;
void reset_counters() {

    s_ctor_counter = 0;
    s_dtor_counter = 0;
    s_move_counter = 0;
}
void print_counters()

{
    std::cout << "ctor: " << s_ctor_counter << ", dtor: " << s_dtor_counter
              << ", move: " << s_move_counter << std::endl;
}

class ptr_test {

  public:
    ptr_test() { s_ctor_counter++; }
    ~ptr_test() { s_dtor_counter++; }
    ptr_test(ptr_test &&other) { s_move_counter++; }
    ptr_test &operator=(ptr_test &&other) {
        s_move_counter++;
        return *this;
    }
    ptr_test(const ptr_test &other) = delete;
    ptr_test &operator=(const ptr_test &other) = delete;
};

int main() {
    using namespace roco::core;
    using heap = roco::core::allocators::heap;

    { // basic
        auto res = uptr<int32_t, heap>::make();
        if (res.has_error()) {
            return 1;
        }
        uptr<int32_t, heap> p(res.take_value());
    }
    { // no copy or move of data
        {
            reset_counters();
            auto res = uptr<ptr_test, heap>::make();
            assert(!res.has_error());
            uptr<ptr_test, heap> p = res.take_value();
        }
        assert(s_ctor_counter == 1);
        assert(s_dtor_counter == 1);
        assert(s_move_counter == 0);
    }
    { // take ownership of ptr
        auto res = allocators::alloc_type<heap, int32_t>(10);
        assert(!res.has_error());
        auto res1 = uptr<int32_t, heap>::take(res.take_value());
        assert(!res1.has_error());
        uptr<int32_t, heap> p = res1.take_value();
        assert((*p) == 10);
    }
    { // move
        auto res = uptr<int32_t, heap>::make(10);
        assert(!res.has_error());
        uptr<int32_t, heap> p = res.take_value();
        int32_t *ptr = p.get();

        uptr<int32_t, heap> p1(std::move(p));
        assert(p.get() == nullptr);
        assert(p1.get() != nullptr);
        assert((*p1) == 10);
        assert(p1.get() == ptr);

        uptr<int32_t, heap> p2 = std::move(p1);
        assert(p1.get() == nullptr);
        assert(p2.get() != nullptr);
        assert((*p2) == 10);
        assert(p2.get() == ptr);
    }
    { // moved is destroyed

        auto res = uptr<int32_t, heap>::make(10);
        assert(!res.has_error());
        uptr<int32_t, heap> p = res.take_value();

        auto res1 = uptr<int32_t, heap>::make(20);
        assert(!res1.has_error());
        uptr<int32_t, heap> p1 = res1.take_value();

        p = std::move(p1);
        assert(*p == 20);
        assert(p1.get() == nullptr);
    }
    { // old obj is delete when new obj is moved in
        reset_counters();
        auto res = uptr<ptr_test, heap>::make();
        assert(!res.has_error());
        uptr<ptr_test, heap> p = res.take_value();
        assert(s_ctor_counter == 1);
        assert(s_dtor_counter == 0);

        auto res1 = uptr<ptr_test, heap>::make();
        assert(!res1.has_error());
        uptr<ptr_test, heap> p1 = res1.take_value();

        assert(s_ctor_counter == 2);
        assert(s_dtor_counter == 0);

        p1 = std::move(p);

        assert(s_ctor_counter == 2);
        assert(s_dtor_counter == 1);

        auto res2 = uptr<ptr_test, heap>::make();
        assert(!res2.has_error());
        uptr<ptr_test, heap> p2 = res2.take_value();

        assert(s_ctor_counter == 3);
        assert(s_dtor_counter == 1);

        uptr<ptr_test, heap> p3(std::move(p2));

        assert(s_ctor_counter == 3);
        assert(s_dtor_counter == 1);
    }
    { // passed null
        auto res = uptr<int32_t, heap>::take(nullptr);
        assert(res.has_error());
        assert(res.take_error() == roco::core::error_enum::is_null_pointer);
    }
    { // manual destruction

        reset_counters();
        auto res = uptr<ptr_test, heap>::make();
        assert(!res.has_error());
        uptr<ptr_test, heap> p = res.take_value();
        assert(s_ctor_counter == 1);
        assert(s_dtor_counter == 0);

        p.destroy();

        assert(s_ctor_counter == 1);
        assert(s_dtor_counter == 1);
        assert(p.get() == nullptr);
    }
    { // manual release
        ptr_test *ptr = nullptr;
        {
            reset_counters();
            auto res = uptr<ptr_test, heap>::make();
            assert(!res.has_error());
            uptr<ptr_test, heap> p = res.take_value();

            ptr = p.release();
            assert(ptr);
            assert(p.get() == nullptr);
        }
        assert(s_dtor_counter == 0);
        allocators::dealloc_type<heap>(ptr);
        assert(s_dtor_counter == 1);
    }
    { // swap

        auto res = uptr<int32_t, heap>::make(10);
        assert(!res.has_error());
        uptr<int32_t, heap> p = res.take_value();
        auto res1 = uptr<int32_t, heap>::make(20);
        assert(!res1.has_error());
        uptr<int32_t, heap> p1 = res1.take_value();

        int32_t *ptr = p.get();
        int32_t *ptr1 = p1.get();

        assert(*ptr == 10);
        assert(*ptr1 == 20);
        swap(p, p1);
        assert(*p == 20);
        assert(*p1 == 10);
        assert(*ptr == 10);
        assert(*ptr1 == 20);
        assert(ptr == p1.get());
        assert(ptr1 == p.get());
    }
    { // double destory is ok

        auto res = uptr<int32_t, heap>::make(10);
        assert(!res.has_error());
        uptr<int32_t, heap> p = res.take_value();
        p.destroy();
        p.destroy();
        assert(p.get() == nullptr);
    }
    { // self assign is ok

        auto res = uptr<int32_t, heap>::make(10);
        assert(!res.has_error());
        uptr<int32_t, heap> p = res.take_value();
        uptr<int32_t, heap> &ref = p;
        p = std::move(ref);
        assert(p.get() != nullptr);
        assert(*p == 10);
    }
    { // moved-out-of object deleted, moved-in-obj is alive

        auto res = uptr<int32_t, heap>::make(10);
        assert(!res.has_error());
        uptr<int32_t, heap> p = res.take_value();
        {
            auto res1 = uptr<int32_t, heap>::make(20);
            assert(!res1.has_error());
            uptr<int32_t, heap> p1 = res1.take_value();
            p = std::move(p1);
        }
        assert(*p == 20);
    }
    return 0;
}
