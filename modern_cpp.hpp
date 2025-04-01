#include <utility>
#pragma "once"

namespace ry {
bool use_weak_ptr();
struct RefCounter {
  int fLvalueCount = 0;
  int fRvalueCount = 0;
  explicit RefCounter();

  RefCounter(const RefCounter &other);
  RefCounter(RefCounter &&other);
  RefCounter &operator=(RefCounter &other);

  RefCounter &operator=(RefCounter &&other);
};

struct ForwardingRefDetector {
  bool fWasRvalue = false;
  explicit ForwardingRefDetector(const int &x);
  explicit ForwardingRefDetector(int &&x);
};

template<typename T>
ForwardingRefDetector do_forward(T &&x) {
    ForwardingRefDetector res{std::forward<T>(x)};
    return res;
}

template<typename T>
ForwardingRefDetector do_not_forward(T &&x) {
    ForwardingRefDetector res{x};
    return res;
}
} // namespace ry
