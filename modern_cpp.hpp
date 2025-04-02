#include <utility>
#pragma "once"

namespace ry {
bool use_weak_ptr();

/**
 * @brief RefCounter counts the number of times lvalue and rvalue constructors
 * and move assignments occur to help illustrate when each is used.
 *
 */
struct RefCounter {
  int fLvalueCount = 0;
  int fRvalueCount = 0;
  explicit RefCounter();

  RefCounter(const RefCounter &other);
  RefCounter(RefCounter &&other);
  RefCounter &operator=(RefCounter &other);

  RefCounter &operator=(RefCounter &&other);
};

/**
 * @brief Records if an lvalue or rvalue overload of its constructor was called.
 * Used to get insight into std::forward behavior
 *
 */
struct ForwardingRefDetector {
  bool fWasRvalue = false;
  explicit ForwardingRefDetector(const int &x);
  explicit ForwardingRefDetector(int &&x);
};

/**
 * @brief Constructs ry::ForwardingRefDetector via std::forward
 *
 * @tparam T - only a template to test forwarding, must pass an int
 * @param x
 * @return ForwardingRefDetector
 */
template <typename T> ForwardingRefDetector do_forward(T &&x) {
  ForwardingRefDetector res{std::forward<T>(x)};
  return res;
}

/**
 * @brief Constructs ry::ForwardingRefDetector without std::forward
 *
 * @tparam T - only a template to test forwarding, must pass an int
 * @param x
 * @return ForwardingRefDetector
 */
template <typename T> ForwardingRefDetector do_not_forward(T &&x) {
  ForwardingRefDetector res{x};
  return res;
}
} // namespace ry
