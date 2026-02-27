#pragma once
#include "Concepts.hpp"

// tag type for keys adapter, used with associative containers
class keys {
 public:
  keys() {}
};

// adapter extracting keys from an associative container
template <Adapter U, typename T>
class Keys {
 private:
  template <typename Iter>
  class Iterator {
   private:
    Iter data_;

   public:
    Iterator(Iter data) : data_(data) {}

    auto operator++() { return ++data_; }
    auto operator--() { return --data_; }

    // dereference returns the key of the pair
    T operator*() { return (*data_).first; }

    bool operator==(Iterator<Iter> other) const { return data_ == other.data_; }
  };

  Iterator<typename U::iterator> begin_;
  Iterator<typename U::iterator> end_;

 public:
  using value_type = T;
  using iterator = Iterator<typename U::iterator>;

  Keys(const U& conteiner)
      : begin_(const_cast<U&>(conteiner).begin()),
        end_(const_cast<U&>(conteiner).end()) {}

  auto begin() const { return begin_; }
  auto end() const { return end_; }
};

// pipe operator for keys: works only on associative containers
template <RAdapter U>
  requires(same_as<typename U::value_type,
                   pair<const typename U::key_type, typename U::mapped_type>>)
Keys<U, typename U::key_type> operator|(const U& conteiner, keys keys_) {
  return Keys<U, typename U::key_type>(conteiner);
}