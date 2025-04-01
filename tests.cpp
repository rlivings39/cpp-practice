#include <gtest/gtest.h>

#include "matrix_ops.hpp"
#include "modern_cpp.hpp"

#define ASSERT_EQ_MATRIX(a, b, nrows, ncols)                                   \
  ASSERT_EQ(a, b) << "Mismatch: \n\n"                                          \
                  << matrix_to_string(a, 2, 2) << "\n"                         \
                  << matrix_to_string(b, 2, 2);

// Test basic 2x2 identity matrix
TEST(MatrixMultiply, SquareIdentity) {
  std::vector<double> a = {1, 2, 3, 4};
  std::vector<double> b = {1, 0, 0, 1};
  auto c = matrix_multiply(a, 2, 2, b, 2, 2);
  ASSERT_EQ_MATRIX(c, a, 2, 2);
}

TEST(ModernCpp, WeakPtr) {
  ASSERT_TRUE(ry::use_weak_ptr());
}

TEST(ModernCpp, MoveAssign) {
  ry::RefCounter rc1, rc2;
  ASSERT_EQ(rc1.fLvalueCount, 0);
  ASSERT_EQ(rc1.fRvalueCount, 0);
  rc2 = rc1;
  ASSERT_EQ(rc2.fLvalueCount, 1);
  ASSERT_EQ(rc2.fRvalueCount, 0);
  rc2 = std::move(rc1);
  ASSERT_EQ(rc2.fLvalueCount, 0);
  ASSERT_EQ(rc2.fRvalueCount, 1);
  rc2 = ry::RefCounter();
  ASSERT_EQ(rc2.fLvalueCount, 0);
  ASSERT_EQ(rc2.fRvalueCount, 1);

  ry::RefCounter rc3(rc1);
  ASSERT_EQ(rc3.fLvalueCount, 1);
  ASSERT_EQ(rc3.fRvalueCount, 0);

  // This does NOT call move c'tor because of copy elision
  ry::RefCounter rc4(ry::RefCounter{});
  ASSERT_EQ(rc4.fLvalueCount, 0);
  ASSERT_EQ(rc4.fRvalueCount, 0);

  // Actually moves and calls move c'tor
  ry::RefCounter rc5(std::move(rc1));
  ASSERT_EQ(rc5.fLvalueCount, 0);
  ASSERT_EQ(rc5.fRvalueCount, 1);

}
