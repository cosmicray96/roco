#pragma once

namespace roco {
namespace core {
namespace algo {

template <typename T> T abs(const T &t) {
  if (t > 0 || t == 0) {
    return t;
  }
  return -t;
}

} // namespace algo
} // namespace core
} // namespace roco
