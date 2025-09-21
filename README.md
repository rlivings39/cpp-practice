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

## C++ parallelism

https://stackoverflow.com/questions/3513045/conditional-variable-vs-semaphore

C++11 added [threading primitives](https://en.cppreference.com/w/cpp/atomic.html) including

```c++
#include <thread>
std::thread
std::this_thread::yield, get_id, sleep_for, sleep_until
#include <mutex>
std::mutex
std::lock_guard
std::unique_lock
std::lock
std::scoped_lock // c++17
#include <atomic>
std::atomic
// Operations
std::atomic_is_lock_free
std::atomic_load, atomic_store, atomic_exchange
std::atomic_fetch*
std::atomic_flag* // atomic boolean and operations
std::memory_order
std::kill_dependency
std::atomic_thread_fence
std::atomic_signal_fence
#include <condition_variable>
std::condition_variable
std::condition_variable_any
std::cv_status
std::notify_all_at_thread_exit
```
### thread

### mutexes and locks

`std::mutex` is a synchronization primitive to protect shared data from simultaneous access from multiple threads. It is non-recursive (i.e. a thread cannot acquire an already owned mutex such as with `std::recursive_mutex`).

* Calling threads **own** a mutex after successfully calling `lock` or `try_lock` until `unlock` is called.
* When a thread owns a lock all other threads will either block on `lock` or receive `false` for `try_lock`.
* Calling threads are not allowed to own the mutex when calling `lock, try_lock`. See `recursive_mutex` if that is needed.

Typically one does not call `lock, try_lock, unlock` directly on a mutex and instead uses an RAII wrapper like `lock_guard, unique_lock, scoped_lock (C++ 17)`.

#### lock_guard

`std::lock_guard` is the simplest RAII mutex ownership wrapper. When constructed with just a mutex, it acquires the lock on construction and releases on destruction.

When constructed with `std::lock_guard lock{m, std::adopt_lock};` on an already locked `m` it acquires ownership without locking and calls `m.unlock()` upon destruction. This is useful for cases when `std::lock` has been used to lock multiple mutexes at once to avoid deadlock. In C++17 using `std::scoped_lock` is preferable.

#### unique_lock

`std::unique_lock` is a mutex ownership wrapper providing more flexibility than `std::lock_guard`. It allows deferred locking, time-constrained locking attempts, recursive locking, transfer of ownership, and use with condition variables.

The constructor accepts a mutex and optionally one of `std::defer_lock, std::try_to_lock, std::adopt_lock` to determine locking behavior. It can also accept a timeout duration or timeout `time_point` to determine when to give up on locking as with `try_lock_for, try_lock_until`.

`std::unique_lock` also provides operations like `lock, try_lock, try_lock_for, try_lock_until, unlock, swap, release` and observes like `mutex, owns_lock, operator bool`.

#### scoped_lock C++17

`std::scoped_lock` is very similar to `std::lock_guard` except that it allows taking ownership of 0 to N mutexes. If more than 1 mutex is passed, a deadlock avoidance algorithm like `std::lock(m1,m2,m3)` is used.

Using `scoped_lock lock{m1,m2,m3};` is shorter than the equivalents with `lock_guard, unique_lock`:

```c++
#include <mutex>
std::mutex m1;
std::mutex m2;
{
    std::lock(m1,m2);
    std::lock_guard<std::mutex> lock1{m1, std::adopt_lock};
    std::lock_guard<std::mutex> lock2{m2, std::adopt_lock};
}
{
    std::unique_lock<std::mutex> lock1{m1, std::defer_lock};
    std::unique_lock<std::mutex> lock2{m2, std::defer_lock};
    std::lock(lock1, lock2);
}
```

### condition_variable

`std::condition_variable` is a synchronization construct used with a `std::mutex` to block one or more threads until another thread modifies a shared variable (the **condition**) and **notifies** the `condition_variable`.

The modifying thread will

1. Acquire the `std::mutex`, usually via `std::lock_guard`
2. Modify the variable with lock ownership
3. Call `notify_one` or `notify_all` on the `std::condition_variable`. The lock may be released before this.

Threads waiting on the condition variable must

1. Acquire a `std::unique_lock<std::mutex>` on the mutex protecting the shared variable
2. Do one of the following
    1. Check the condition in case it was already modified
    2. Call `wait, wait_for, wait_until` on the `condition_variable`. This releases the mutex and suspends the thread until later notification, a timeout expires, or a spurious wake up happens, then acquires the mutex before returning control
    3. Check the condition and resume waiting if not satisfied

A **spurious wake up** happens when a thread waiting on a condition variable wakes up and finds that the condition is still unsatisfied. This can happen when between the time that the condition variable was signaled and the thread wakes up another thread already woke up and invalidated the condition. So there is a race condition when multiple threads wait on a single condition variable. The first thread to run wins and the rest see a spurious wake up.

Because of this, threads must verify the condition upon waking from waiting for a condition variable.

In C++ `std::condition_variable::wait, wait_for, wait_until` all accept predicates which allow ignoring spurious wake ups.

### atomic

### Memory order, fences, etc.

Include x64 basic info

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
