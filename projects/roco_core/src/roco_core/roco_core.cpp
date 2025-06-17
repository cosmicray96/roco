#include "roco_core/roco_core.hpp"

namespace roco {
namespace core {

void crash_program(const char *msg) {
    if (msg) {
        std::cerr << msg << std::endl;
    }
    std::exit(EXIT_FAILURE); // std::abort();
}

int roco_core::add(int a, int b) { return a + b; }
} // namespace core
} // namespace roco
