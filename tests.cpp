#include <algorithm>
#include <gtest/gtest.h>

#include "matrix_ops.hpp"
#include "modern_cpp.hpp"
#include <optional>
#include <type_traits>
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

namespace {
/**
 * @brief Helper class to make an overload set: overloaded([](){}, [](){}, ...)
 *
 * @tparam Ts
 */
template <class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};
// explicit deduction guide (not needed as of C++20)
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

/**
 * @brief Demonstrates using constexpr if for a variant visitor
 *
 * @tparam InT
 * @param var
 * @return std::string
 */
template <typename InT> std::string constexpr_if_overload(InT &&var) {
  using T = std::decay_t<decltype(var)>;
  if constexpr (std::is_same_v<T, std::string>) {
    return "string";
  } else if constexpr (std::is_same_v<T, int>) {
    return "int";
  } else {
    static_assert(false, "Incomplete visitor");
  }
}
} // namespace

TEST(ModernCpp, Variant) {
  string_or_int_t var{"hello"};
  ASSERT_EQ(var.index(), 0);
  ASSERT_TRUE(std::holds_alternative<std::string>(var));
  ASSERT_FALSE(std::holds_alternative<int>(var));
  ASSERT_EQ(std::get<0>(var), "hello");
  std::string *str = std::get_if<std::string>(&var);
  ASSERT_NE(str, nullptr);
  int *i = std::get_if<int>(&var);
  ASSERT_EQ(i, nullptr);
  ASSERT_EQ(*str, "hello");
  ASSERT_THROW(std::get<1>(var), std::bad_variant_access);
  ASSERT_EQ(std::variant_size_v<string_or_int_t>, 2);

  // Visitation doc https://en.cppreference.com/w/cpp/utility/variant/visit2

  // Visit using a class with N operator() overloads
  auto variant_visitor = overloaded{[](std::string) { return "string"; },
                                    [](int) { return "int"; }};
  ASSERT_EQ(std::visit(variant_visitor, var), "string");

  // Visit doing dispatch with if constexpr
  ASSERT_EQ(
      std::visit([](auto &&val) { return constexpr_if_overload(val); }, var),
      "string");

  var = 42;
  ASSERT_EQ(var.index(), 1);
  ASSERT_FALSE(std::holds_alternative<std::string>(var));
  ASSERT_TRUE(std::holds_alternative<int>(var));
  ASSERT_EQ(std::get<1>(var), 42);
  str = std::get_if<std::string>(&var);
  ASSERT_EQ(str, nullptr);
  i = std::get_if<int>(&var);
  ASSERT_EQ(*i, 42);
  ASSERT_THROW(std::get<0>(var), std::bad_variant_access);

  // Visit using a class with N operator() overloads
  ASSERT_EQ(std::visit(variant_visitor, var), "int");

  // Visit doing dispatch with if constexpr
  ASSERT_EQ(
      std::visit([](auto &&val) { return constexpr_if_overload(val); }, var),
      "int");

  // Visit with a visitor that returns the visitor type to transform a vector
  std::vector<string_or_int_t> v{1, "hello"};
  std::transform(std::begin(v), std::end(v), std::begin(v), [](auto &&v) {
    return std::visit([](auto &&arg) -> string_or_int_t { return arg + arg; },
                      v);
  });
  ASSERT_EQ(std::get<1>(v[0]), 2);
  ASSERT_EQ(std::get<0>(v[1]), "hellohello");
}

/**
 * @brief Demonstrate virtual inheritance
 *
 */
struct Base {
  int x = 0;
};
// DerivedV1/2 share one instance of x
struct DerivedV1 : public virtual Base {};
struct DerivedV2 : virtual public Base {};
// DerivedNonV has its own instance of x
struct DerivedNonV : public Base {};
struct RollUp : public DerivedV1, DerivedV2, DerivedNonV {
  void incrementXs() {
    DerivedV1::x++;
    DerivedV2::x++;
    DerivedNonV::x++;
  }

  auto getXs() {
    return std::make_tuple(DerivedV1::x, DerivedV2::x, DerivedNonV::x);
  }
};

TEST(ModernCpp, VirtualInheritance) {
  RollUp r;
  auto [x1, x2, x3] = r.getXs();
  ASSERT_EQ(x1, 0);
  ASSERT_EQ(x2, 0);
  ASSERT_EQ(x3, 0);

  r.incrementXs();
  ASSERT_EQ(r.getXs(), std::make_tuple(2, 2, 1));
  // Here's how to disambiguate a member...Also verify that the memory locations
  // are as expected
  ASSERT_EQ(r.DerivedV1::x, 2);
  ASSERT_EQ(&r.DerivedV1::x, &r.DerivedV2::x);
  ASSERT_NE(&r.DerivedV1::x, &r.DerivedNonV::x);
}

struct rule_of_5 {
  int *x;
  rule_of_5(int val) {
    x = new int(val);
  }
  rule_of_5(const rule_of_5 &other) : rule_of_5(*other.x) {}
  rule_of_5(rule_of_5 &&other) noexcept : x(std::exchange(other.x, nullptr)) {}
  rule_of_5 &operator=(const rule_of_5 &other) {
    // Copy and move assign from other to avoid storage reuse
    *this = rule_of_5(other);
    return *this;
  }
  rule_of_5 &operator=(rule_of_5 &&other) {
    // Copy and swap to avoid storage reuse
    rule_of_5 tmp(other);
    std::swap(tmp.x, this->x);
    return *this;
  }
  ~rule_of_5() {
    delete x;
  };
};

TEST(ModernCpp, RuleOf5) {
  {
    rule_of_5 r(42);
    ASSERT_NE(r.x, nullptr);
    ASSERT_EQ(*r.x, 42);

    // Copy construct and ensure things are appropriate. Also trigger d'tor to
    // ensure no crashes.
    {
      rule_of_5 rcopy(r);
      ASSERT_NE(rcopy.x, nullptr);
      ASSERT_NE(rcopy.x, r.x);
      ASSERT_EQ(*rcopy.x, 42);
    }
    // Copy assign and determine the same
    {
      rule_of_5 rcopy = r;
      ASSERT_NE(rcopy.x, nullptr);
      ASSERT_NE(rcopy.x, r.x);
      ASSERT_EQ(*rcopy.x, 42);
    }
    // Move construct and ensure things work properly
    {
      rule_of_5 rnew(42);
      rule_of_5 rcopy(std::move(rnew));
      ASSERT_NE(rcopy.x, nullptr);
      ASSERT_EQ(rnew.x, nullptr);
      ASSERT_NE(rcopy.x, rnew.x);
      ASSERT_EQ(*rcopy.x, 42);
    }
    // Move assign and ensure things work properly
    {
      rule_of_5 rnew(42);
      rule_of_5 rcopy = std::move(rnew);
      ASSERT_NE(rcopy.x, nullptr);
      ASSERT_EQ(rnew.x, nullptr);
      ASSERT_NE(rcopy.x, rnew.x);
      ASSERT_EQ(*rcopy.x, 42);
    }
  }
  // Now check self assignment
  {
    rule_of_5 r1(42);
    int *xorig = r1.x;
    ASSERT_EQ(*r1.x, 42);
    r1 = r1;
    ASSERT_EQ(*r1.x, 42);
    // Note: this implementation does change memory after assignment
    ASSERT_NE(r1.x, xorig);
  }
  // Now check self move assignment
  {
    rule_of_5 r1(42);
    int *xorig = r1.x;
    ASSERT_EQ(*r1.x, 42);
    r1 = std::move(r1);
    ASSERT_EQ(*r1.x, 42);
    // Note: this implementation does change memory after assignment
    ASSERT_NE(r1.x, xorig);
  }
}

TEST(ModernCpp, Lambdas) {
  int val{42}, val2{44};
  // Renaming capture in lambda by value and by reference
  auto f = [y = val, &z = val2](int arg) {
    ++z;
    return arg + y;
  };
  ASSERT_EQ(f(12), 54);
  ASSERT_EQ(val2, 45);
}
