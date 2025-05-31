#pragma once

namespace roco {
namespace core {
namespace collection {

template <typename t_elem> class iterator {
public:
  iterator() = default;
  virtual ~iterator() = default;

public:
  virtual void inc() = 0;
  virtual bool equals(const iterator &other) = 0;
  virtual t_elem get() = 0;
};

} // namespace collection
} // namespace core
} // namespace roco
