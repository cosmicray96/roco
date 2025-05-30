#include "roco_core/roco_core.hpp"
#include <cassert>

int main() {

    assert(1 == 1);
    assert(roco_core::roco_core::add(1, 2) == 3);

    return 0;
}
