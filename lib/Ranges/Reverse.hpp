#pragma once
#include "Concepts.hpp"

// tag type for reverse adapter; no data stored
class reverse {
 public:
  reverse() {}
};

// adapter implementing reverse iteration order
template <Adapter U, typename T>
class Reverse {
 private:
  // inner iterator that decrements on ++ to walk backwards
  template <typename Iter>
  class Iterator {
   private:
    Iter data_; // underlying iterator position

   public:
    Iterator(Iter data) : data_(data) {}

    auto operator++() { return --data_; }
    auto operator--() { return ++data_; }
    T operator*() { return *data_; }
    bool operator==(Iterator<Iter> other) const { return data_ == other.data_; }
  };

  Iterator<typename U::iterator> begin_;
  Iterator<typename U::iterator> end_;

 public:
  using value_type = T;
  using iterator = Iterator<typename U::iterator>;

  // initialize begin_ at end(), then increment to first valid element (last of container)
  Reverse(const U& conteiner)
      : begin_(const_cast<U&>(conteiner).end()),
        end_(const_cast<U&>(conteiner).begin()) {
    ++begin_;
    ++end_;
  }

  auto begin() const { return begin_; }
  auto end() const { return end_; }
};

// pipe operator: container | reverse()
template <RAdapter U>
Reverse<U, typename U::value_type> operator|(const U& conteiner,
                                             reverse reverse_) {
  return Reverse<U, typename U::value_type>(conteiner);
}