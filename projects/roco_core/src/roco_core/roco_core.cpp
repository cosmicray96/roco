#include "roco_core/roco_core.hpp"
#include <cstddef>

namespace roco {
namespace core {

void crash_program(const char *msg) {
  if (msg) {
    std::cerr << msg << std::endl;
  }
  std::exit(EXIT_FAILURE); // std::abort();
}
void crash_program(error_enum error, const char *msg) {
  std::cerr << "ERR: " << (size_t)error;
  if (msg) {
    std::cerr << " msg: " << msg << std::endl;
  }
  std::cerr << std::endl;
  std::exit(EXIT_FAILURE); // std::abort();
}

int roco_core::add(int a, int b) { return a + b; }
} // namespace core
} // namespace roco
