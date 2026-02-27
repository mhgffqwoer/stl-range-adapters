#pragma once
#include <iostream>

using std::invocable;
using std::invoke_result_t;
using std::pair;
using std::same_as;

// concept for a generic adapter: must provide begin()/end() that can advance
template <typename T>
concept Adapter = requires(T a) {
  ++a.begin();    // forward iterator
  *a.end();       // dereferenceable
};

// concept for adapters that also support reverse iteration (needed by reverse)
template <typename T>
concept RAdapter = requires(T a) {
  ++a.begin();
  *a.end();
  --a.end();      // decrementable
};
