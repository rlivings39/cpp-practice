/**
 * Topics to cover:
 *
 * [x] Smart pointers, especially weak_ptr
 * [] Smart pointers in graphs/cycles
 * [] r-value references, universal references
 * [] Argument packs/variadiac templates, fold expressions. Make a tuple
 * [] Private, virtual, etc. inheritance
 * [] Rule of 0,3,5
 * [] Do some operator overloading
 * [] Option and variant
 * [] Basic ranges
 * [] Lambdas w/ captures
 * [] Threading and parallelism, thread pools, condition vars, atomics
 *   Useful
 * https://stackoverflow.com/questions/3513045/conditional-variable-vs-semaphore
 * [] Locks, mutexes, semaphores
 * [] Using override, default, delete, const on methods
 * [] C'tors, assignment, d'tors (copy and move)
 * [] auto, decltype auto, std::decay, etc.
 * [] Initializer lists, uniform initialization
 * [] ADL
 * [] Variable templates
 * [] Generic lambdas
 * [] if-init: if(auto it = map.find(key); it != map.end()) { use(it); }
 * [] Structured binding declarations
 * [] Inline variables for single definition of globals and class statics in
 * headers
 * [] if constexpr
 * [] Improved lambda to capture with [x = std::move(obj)] { use(x); }
 * [] std::filesystem
 * [] <string_view>, <any>, <optional>, <variant>, <memory_resource>
 *
 * C++ 20
 * [] concepts
 * [] ranges
 * [] coroutines
 * [] constexpr and consteval
 * []
 * https://en.cppreference.com/w/cpp/11
 * https://en.cppreference.com/w/cpp/14
 * https://en.cppreference.com/w/cpp/17
 * https://en.cppreference.com/w/cpp/20
 *
 */

#include "modern_cpp.hpp"
#include <iostream>
#include <memory>

namespace {
template <typename T> std::shared_ptr<T> get_shared(std::weak_ptr<T> wp) {
  auto res = wp.lock();
  if (res) {
    std::cout << "Got a reference\n";
  } else {
    std::cout << "Failed to get a reference\n";
  }
  std::cout << "expired returned: " << std::boolalpha << wp.expired() << "\n";
  return res;
}
} // namespace
namespace ry {

bool use_weak_ptr() {
  std::weak_ptr<int> wp1;
  {
    std::shared_ptr<int> sp1 = std::make_shared<int>(42);
    wp1 = sp1;
    get_shared(wp1);
  }
  get_shared(wp1);

  return true;
}
} // namespace ry
