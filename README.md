# STL Range Adapters

A small library providing lazy adapters for standard containers and algorithms, enabling functional-style pipelines while keeping computations deferred and objects lightweight.

## Description

The package offers a set of adapters (`filter`, `transform`, `take`, `drop`, `reverse`, `keys`, `values`) that can be used
in combination with the `|` operator, similar to Unix streams or modern range libraries.

**Features:**

- **Lazy evaluation**: elements are not processed until iterated.
- **Non-owning**: adapters hold only references to the original container.
- **Easy integration**: works with any iterable container, requirements enforced via `static_assert`.
- **Composable**: adapters can be chained arbitrarily.

## How it works

Each adapter is a wrapper object around a pair of iterators (begin, end).
Adapters implement their own proxy iterators which, on increment or dereference,
apply the provided predicate, transformation, or simply advance.

1. **filter** - skips elements that do not satisfy a predicate.
2. **transform** - returns the result of a transformer function when dereferenced.
3. **take** - limits the number of returned elements.
4. **drop** - skips the first N elements.
5. **reverse** - iterates the container in reverse order.
6. **keys / values** - operate on associative containers, returning only keys or values.

All adapters provide `begin()`/`end()` and can be used with STL ranges and algorithms.

## Project structure

```
/ (root directory)
├─ CMakeLists.txt
├─ README.md                # this description (now in English)
├─ bin/
│   ├─ CMakeLists.txt
│   └─ main.cpp             # library demonstration
├─ lib/                     # the library itself
│   ├─ CMakeLists.txt
│   └─ Ranges/              # adapter implementation
│       ├─ Concepts.hpp      # concepts and requirements
│       ├─ Drop.hpp
│       ├─ Filter.hpp
│       ├─ Keys.hpp
│       ├─ Ranges.cpp        # single compilation unit for testing
│       ├─ Ranges.hpp
│       ├─ Reverse.hpp
│       ├─ Take.hpp
│       ├─ Transform.hpp
│       └─ Values.hpp
└─ tests/
    ├─ CMakeLists.txt
    └─ tests.cpp            # usage examples and checks
```

## Usage examples

### filter

```cpp
std::vector<int> v = {1,2,3,4,5,6};
auto odd = v | filter([](int x){ return x % 2 == 1; });
// output: 1 3 5
for (int i : odd) std::cout << i << " ";
```

### transform

```cpp
std::vector<int> v = {1,2,3,4,5,6};
auto squares = v | transform([](int x){ return x * x; });
// output: 1 4 9 16 25 36
for (int i : squares) std::cout << i << " ";
```

### take

```cpp
std::vector<int> v = {1,2,3,4,5,6};
auto firstThree = v | take(3);
// output: 1 2 3
for (int i : firstThree) std::cout << i << " ";
```

### drop

```cpp
std::vector<int> v = {1,2,3,4,5,6};
auto afterOne = v | drop(1);
// output: 2 3 4 5 6
for (int i : afterOne) std::cout << i << " ";
```

### reverse

```cpp
std::vector<int> v = {1,2,3,4,5,6};
auto rev = v | reverse();
// output: 6 5 4 3 2 1
for (int i : rev) std::cout << i << " ";
```

### keys / values

```cpp
std::map<std::string,int> m = {{"a",1},{"b",2},{"c",3}};
for (auto k : m | keys())   std::cout << k << " "; // a b c
for (auto val : m | values()) std::cout << val << " "; // 1 2 3
```

## License

The project is open; see `LICENSE` file for terms (MIT by default).
