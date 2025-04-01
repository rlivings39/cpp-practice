#pragma "once"

namespace ry {
bool use_weak_ptr();
struct RefCounter {
    int fLvalueCount = 0;
    int fRvalueCount = 0;
    RefCounter();

    RefCounter &operator=(RefCounter &other);

    RefCounter &operator=(RefCounter &&other);
  };

}
