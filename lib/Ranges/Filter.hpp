#pragma once
#include "Concepts.hpp"

// user-facing tag for filter adapter; stores the predicate function
template <typename Func>
class filter {
 public:
  Func func_;          // predicate to test elements
  filter(Func func) : func_(func) {}
};

template <Adapter U, typename T, typename Func>
class Filter {
 private:
  // iterator proxy that applies the filter predicate lazily
  template <typename Iter>
  class Iterator {
   private:
    Iter data_;    // current position in underlying container
    Func func_;    // predicate to test elements
    Iter end_;     // one-past-end of underlying container
    Iter rend_;    // reverse-bound (begin of container)

   public:
    Iterator(Iter data, Func func, Iter end, Iter rend)
        : data_(data), func_(func), end_(end), rend_(rend) {
      // adjust rend_ to point to last valid element
      --rend_;
    }

    // advance to next element satisfying predicate
    auto operator++() {
      ++data_;
      while (data_ != end_ && !func_(*data_)) ++data_;
      return data_;
    }

    // move backward to previous element satisfying predicate
    auto operator--() {
      --data_;
      while (data_ != rend_ && !func_(*data_)) --data_;
      return data_;
    }

    // dereference returns the underlying value (no transform)
    T operator*() { return *data_; }

    bool operator==(Iterator<Iter> other) const { return data_ == other.data_; }
  };

  Iterator<typename U::iterator> begin_;
  Iterator<typename U::iterator> end_;
  Func func_;

 public:
  using value_type = T;
  using iterator = Iterator<typename U::iterator>;

  // constructor stores container iterators and predicate
  Filter(const U& conteiner, filter<Func> filter)
      : begin_(const_cast<U&>(conteiner).begin(), filter.func_,
               const_cast<U&>(conteiner).end(),
               const_cast<U&>(conteiner).begin()),
        end_(const_cast<U&>(conteiner).end(), filter.func_,
             const_cast<U&>(conteiner).end(),
             const_cast<U&>(conteiner).begin()),
        func_(filter.func_) {}

  // begin() adjusts to the first matching element
  auto begin() {
    while (begin_ != end_ && !func_(*begin_)) ++begin_;
    return begin_;
  }

  auto end() const { return end_; }
};

// pipe operator implementation: container | filter(predicate)
// returns a Filter wrapper with appropriate types
template <Adapter U, typename Func>
  requires invocable<Func, typename U::value_type>
Filter<U, typename U::value_type, Func> operator|(const U& conteiner,
                                                  filter<Func> filter) {
  return Filter<U, typename U::value_type, Func>(conteiner, filter);
}