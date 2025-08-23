# cpp-practice

Some exercises for me to shake off the C++ cobwebs and practice LeetCode.

Build with CMake

```bash
cmake --build ./build --config Debug --target all
ctest --test-dir build
```
and run the tests in `./build/cpp-practice-test` to see if things are working right.

The unit test file [tests.cpp](tests.cpp) is a great place to start reading. It covers a matrix multiplication algorithm as well as review on modern C++ features.

[test_graph.cpp](test_graph.cpp) has tests for some graph algorithms

## Modern C++ features to review

Topics to cover:

* [x] Smart pointers, especially weak_ptr
* [x] r-value references, universal references
* [x] Private, virtual, etc. inheritance
* [x] Rule of 0,3,5 https://en.cppreference.com/w/cpp/language/rule_of_three
* [ ] Do some operator overloading
* [ ] Threading and parallelism, thread pools, condition vars, atomics
    * Useful https://stackoverflow.com/questions/3513045/conditional-variable-vs-semaphore
* [ ] Locks, mutexes, semaphores
* [x] Optional and variant
* [x] Lambdas w/ captures and init-capture `[x = std::move(obj)] { use(x); }`
* [x] Using override, default, delete, const on methods
* [x] C'tors, assignment, d'tors (copy and move)
* [ ] auto, decltype auto, std::decay, etc.
* [ ] Initializer lists, uniform initialization
* [ ] Argument packs/variadic templates, fold expressions. Make a tuple
    * https://en.cppreference.com/w/cpp/language/pack has a lot of good detail. Crazy complicated.
    * https://en.cppreference.com/w/cpp/language/fold
* [ ] Smart pointers in graphs/cycles
* [ ] Basic ranges
* [ ] ADL
* [ ] Variable templates
* [ ] Generic lambdas
* [ ] if-init: `if(auto it = map.find(key); it != map.end()) { use(it); }`
* [ ] Structured binding declarations
* [ ] Inline variables for single definition of globals and class statics in headers
* [ ] `if constexpr`
* [ ] std::filesystem
* [ ] `<string_view>, <any>, <optional>, <variant>, <memory_resource>`
* C++ 20
* [ ] concepts
* [ ] ranges
* [ ] coroutines
* [ ] constexpr and consteval

General standard version references

* https://en.cppreference.com/w/cpp/11
* https://en.cppreference.com/w/cpp/14
* https://en.cppreference.com/w/cpp/17
* https://en.cppreference.com/w/cpp/20

## learncpp.com review

