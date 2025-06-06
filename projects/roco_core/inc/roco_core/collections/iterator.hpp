#pragma once

namespace roco {
namespace core {
namespace collections {

template <typename t_elem> class iterator {
  public:
    iterator() = default;
    virtual ~iterator() = default;

  public:
    virtual t_elem &operator*() = 0;
    virtual void inc() = 0;
    virtual bool equals(const iterator &other) = 0;
    virtual t_elem &get() = 0;
};

} // namespace collections
} // namespace core
} // namespace roco
