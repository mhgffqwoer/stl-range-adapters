#pragma once
#include "Concepts.hpp"

// tag object for take adapter; stores how many elements to keep
class take {
 public:
  size_t number_;
  take(size_t number) : number_(number) {}
};

// adapter implementation that restricts iteration to first N elements
template <Adapter U, typename T>
class Take {
 private:
  typename U::iterator begin_;
  typename U::iterator end_;
  size_t number_;

 public:
  using value_type = T;
  using iterator = typename U::iterator;

  // compute end iterator by advancing N steps from container.begin()
  Take(const U& conteiner, take take_) {
    begin_ = const_cast<U&>(conteiner).begin();
    number_ = take_.number_;
    end_ = begin_;
    auto finish = const_cast<U&>(conteiner).end();
    for (size_t i = 0; end_ != finish && i < number_; ++i) ++end_;
  }

  auto begin() const { return begin_; }
  auto end() const { return end_; }
};

// pipe operator for take: container | take(N)
template <Adapter U>
Take<U, typename U::value_type> operator|(const U& conteiner, take take_) {
  return Take<U, typename U::value_type>(conteiner, take_);
}