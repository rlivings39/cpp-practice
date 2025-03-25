#include <gtest/gtest.h>

#include "matrix_ops.hpp"

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
