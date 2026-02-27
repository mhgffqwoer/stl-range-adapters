#pragma once
#include "Concepts.hpp"

// user-facing tag for transform adapter; holds transformation function
template <typename Func>
class transform {
 public:
  Func func_;          // unary function applied to elements
  transform(Func func) : func_(func) {}
};

template <Adapter U, typename T, typename Func>
class Transform {
 private:
  // iterator wrapper that applies transformation on dereference
  template <typename Iter>
  class Iterator {
   private:
    Iter data_;    // underlying iterator
    Func func_;    // transformation function

   public:
    Iterator(Iter data, Func func) : data_(data), func_(func) {}

    Iterator() = default;

    auto operator++() { return ++data_; }
    auto operator--() { return --data_; }

    // when dereferenced, apply func to underlying element
    T operator*() { return func_(*data_); }

    bool operator==(Iterator<Iter> other) const { return data_ == other.data_; }
  };

  Iterator<typename U::iterator> begin_;
  Iterator<typename U::iterator> end_;

 public:
  using value_type = T;
  using iterator = Iterator<typename U::iterator>;

  // construct from container; store begin/end and function
  Transform(const U& conteiner, transform<Func> trans)
      : begin_(const_cast<U&>(conteiner).begin(), trans.func_),
        end_(const_cast<U&>(conteiner).end(), trans.func_) {}

  auto begin() const { return begin_; }
  auto end() const { return end_; }
};

// pipe operator for transform: returns Transform wrapper
template <Adapter U, typename Func>
  requires invocable<Func, typename U::value_type>
Transform<U, invoke_result_t<Func, typename U::value_type>, Func> operator|(
    const U& conteiner, transform<Func> trans) {
  return Transform<U, invoke_result_t<Func, typename U::value_type>, Func>(
      conteiner, trans);
}