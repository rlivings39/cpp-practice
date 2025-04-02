#include <gtest/gtest.h>

#include "matrix_ops.hpp"
#include "modern_cpp.hpp"
#include <optional>
#include <variant>

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

// C++11/14/17/20 feature review
TEST(ModernCpp, WeakPtr) {
  ASSERT_TRUE(ry::use_weak_ptr());
}

TEST(ModernCpp, MoveAssign) {
  ry::RefCounter rc1, rc2;
  // Ensure expected default state
  ASSERT_EQ(rc1.fLvalueCount, 0);
  ASSERT_EQ(rc1.fRvalueCount, 0);

  // Simple lvalue assignment
  rc2 = rc1;
  ASSERT_EQ(rc2.fLvalueCount, 1);
  ASSERT_EQ(rc2.fRvalueCount, 0);

  // rvalue assignment
  rc2 = std::move(rc1);
  ASSERT_EQ(rc2.fLvalueCount, 0);
  ASSERT_EQ(rc2.fRvalueCount, 1);

  // assignment from temporary
  rc2 = ry::RefCounter();
  ASSERT_EQ(rc2.fLvalueCount, 0);
  ASSERT_EQ(rc2.fRvalueCount, 1);

  // copy c'tor from lvalue
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

TEST(ModernCpp, Forwarding) {
  // c'tor with rvalue input
  ry::ForwardingRefDetector fr1{1};
  ASSERT_EQ(fr1.fWasRvalue, true);

  // c'tor with lvalue input
  int x{42};
  ry::ForwardingRefDetector fr2{x};
  ASSERT_EQ(fr2.fWasRvalue, false);

  // With forwarding passing an rvalue forwards an rvalue, same for lvalue
  ry::ForwardingRefDetector fr3 = ry::do_forward(1);
  ASSERT_EQ(fr3.fWasRvalue, true);

  ry::ForwardingRefDetector fr4 = ry::do_forward(x);
  ASSERT_EQ(fr4.fWasRvalue, false);

  // Without forwarding passing an rvalue or lvalue passes lvalue
  ry::ForwardingRefDetector fr5 = ry::do_not_forward(x);
  ASSERT_EQ(fr5.fWasRvalue, false);

  ry::ForwardingRefDetector fr6 = ry::do_not_forward(x);
  ASSERT_EQ(fr6.fWasRvalue, false);
}

// Helper to test optional
std::optional<std::string> maybe_create(bool b) {
  if (b) {
    return "created";
  }
  return std::nullopt;
}

TEST(ModernCpp, Optional) {
  std::optional<std::string> o;
  ASSERT_FALSE(o);

  o = maybe_create(false);
  ASSERT_FALSE(o);
  ASSERT_FALSE(o.has_value());
  ASSERT_EQ(o.value_or("not created"), "not created");
  // *o and o-> are undefined here

  o = maybe_create(true);
  ASSERT_TRUE(o);
  ASSERT_TRUE(o.has_value());
  ASSERT_EQ(o.value(), "created");
  ASSERT_EQ(o.value_or("asdf"), "created");
  ASSERT_EQ(*o, "created");
  ASSERT_STREQ(o->c_str(), "created");
}

using string_or_int_t = std::variant<std::string, int>;

TEST(ModernCpp, Variant) {
  string_or_int_t s{"hello"};
  ASSERT_EQ(s.index(), 0);
  ASSERT_TRUE(std::holds_alternative<std::string>(s));
  ASSERT_FALSE(std::holds_alternative<int>(s));
}
