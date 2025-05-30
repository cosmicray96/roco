#pragma once

#include <concepts>
#include <cstddef>
#include <memory>

namespace roco {
namespace core {
namespace collection {

template <typename T>
concept is_collection_elem = std::copyable<T> && std::movable<T> && std::default_initializable<T>;

template <typename T>
concept is_collection = requires(T a) {
    { a.count() } -> std::convertible_to<size_t>;
    T::t_elem_type;
} && is_collection_elem<typename T::t_elem>;

/*
class is_collection_i {
    class concept_c {
      public:
        concept_c() = default;
        virtual ~concept_c() = default;
        virtual size_t count() = 0;
    };

    template <typename T> class model : public concept_c {
      public:
        model(T &o) : obj(o) {}
        virtual size_t count() override { return obj.count(); }

      public:
        T obj;
    };

  public:
    template <typename T>
        requires is_collection<T>
    is_collection_i(std::unique_ptr<T> t) : self(t) {}

  public:
    std::unique_ptr<concept_c> self;
};

class is_collection_elem_i {
  public:
    is_collection_elem_i() = default;
    is_collection_elem_i(const is_collection_elem_i &other) = default;
    is_collection_elem_i(is_collection_elem_i &&other) = default;
    virtual ~is_collection_elem_i() = default;
};
*/

} // namespace collection
} // namespace core
} // namespace roco
