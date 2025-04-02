// Practice and review of C++ 11/14/17/20 features

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

RefCounter::RefCounter() {}

RefCounter::RefCounter(const RefCounter &other)
    : fLvalueCount(other.fLvalueCount + 1), fRvalueCount(other.fRvalueCount) {}

RefCounter::RefCounter(RefCounter &&other)
    : fLvalueCount(other.fLvalueCount), fRvalueCount(other.fRvalueCount + 1) {}

RefCounter &RefCounter::operator=(RefCounter &other) {
  this->fLvalueCount = other.fLvalueCount + 1;
  this->fRvalueCount = other.fRvalueCount;
  return *this;
}

RefCounter &RefCounter::operator=(RefCounter &&other) {
  this->fLvalueCount = other.fLvalueCount;
  this->fRvalueCount = other.fRvalueCount + 1;
  return *this;
}

ForwardingRefDetector::ForwardingRefDetector(const int &x)
    : fWasRvalue(false) {}
ForwardingRefDetector::ForwardingRefDetector(int &&x) : fWasRvalue(true) {}

} // namespace ry
