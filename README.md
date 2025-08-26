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

* [ ] Google common C++ interview questions
* [ ] Various casts and their purpose: static, const, reinterp, dynamic, c
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

### Initialization

As of c++17 `int b=5; int c(6);, int d{7};, int e{};` are all basically equivalent...except when they're not.

1. List initialization `{}` disallows narrowing conversions
2. Copy init `T t = val` allows non-explicit conversion functions.
3. `{}` prefers matching `std::initializer_list` constructors

Class template argument deduction in C++17 occurs in a few places

* Declarations with initializers
* Function-style casts `std::lock_guard(some_mutex)`
* New expressions

### Casts

* `const_cast` - Allows for adding/removing `const` from an expression. Typically we're dealing with references/pointers to `const`. It is UB to write to a const object after `const_cast`
* `static_cast` - General value conversions
    * `static_cast<Derived&>(base_reference)` is allowed if base_reference is a sub-object of a `Derived`. Similar for rvalue references
    *  `static_cast<void>(foo)` is a discarded value expression
    * `static_cast<target_type>(expression)` is equivalent to `target-type temp(expression);` for some invented `temp`
    * Pointer to base can be cast to pointer to derived when expression is a subobject of a derived. Except for `virtual` and other bad cases. No runtime check is done.
    * Derived to base is allowed and is implicit
    * Pointer to void can be converted to pointer to `T` provided that alignment requirements are met
* `dynamic_cast` - Safely casts up, down, and sideways in inheritance hierarchy
    * Adding CV qualifiers is legal as with `static_cast` and implicit
    * Pointer to derived can be cast to pointer to base as with `static_cast` and implicit. Ditto for references. **Upcast**
    * `nullptr` of a polymorphic type can covert to another pointer
    * Otherwise operand must be a pointer or reference to an object of polymorphic type within its lifetime or within c'tor/d'tor.
    * Pointer to void is converted to the most-derived object pointed to
    * Otherwise a runtime check is performed to see if the pointed to/referred to type of the operand can be converted to the target type.
        * A valid **downcast** from base to derived is allowed if base refers/points to a derived
        * A **cross cast/side cast** is valid for something like `struct D: A, B` if you have a reference/pointer to `A` with super object `D` you can cast to reference/pointer to `B`
        * Invalid pointer casts return nullptr. Invalid reference casts throw `std::bad_cast`
* `reinterpret_cast` - Generally results in no CPU instructions and just tells the compiler to treat an object as having another type
    * Pointers can be converted to integer/enum types large enough to hold the value e.g. `std::uintptr_t`
    * Integer or enum can be converted to pointer
    * `nullptr_t` can be converted to integer type. Nothing can be converted to `nullptr_t`
    * Pointer types are convertible as if `static_cast<cv T2*>(static_cast<cv void*>(expression))`. They must be alignment compatible. Dereferencing is OK only when type-accessible. This basically means `char/unsigned char`, the same type, or the corresponding signed/unsigned type
    * `std::memcpy` should be used to reinterpret bytes of unrelated types

### Next

https://www.learncpp.com/cpp-tutorial/syntax-and-semantic-errors/
