#pragma once
#include "Concepts.hpp"

// tag type for drop adapter; counts elements to skip
class drop {
 public:
  size_t number_;
  drop(size_t number) : number_(number) {}
};

// adapter implementation that skips N elements at start
template <Adapter U, typename T>
class Drop {
 private:
  typename U::iterator begin_;
  typename U::iterator end_;
  size_t number_;

 public:
  using value_type = T;
  using iterator = typename U::iterator;

  // advance begin_ by N or until container end
  Drop(const U& conteiner, drop drop_) {
    begin_ = const_cast<U&>(conteiner).begin();
    number_ = drop_.number_;
    end_ = const_cast<U&>(conteiner).end();
    for (size_t i = 0; begin_ != end_ && i < number_; ++i) ++begin_;
  }

  auto begin() const { return begin_; }
  auto end() const { return end_; }
};

// pipe operator for drop: container | drop(N)
template <Adapter U>
Drop<U, typename U::value_type> operator|(const U& conteiner, drop drop_) {
  return Drop<U, typename U::value_type>(conteiner, drop_);
}