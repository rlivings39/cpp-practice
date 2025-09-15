
#include "data_structures.hpp"
#include <algorithm>
#include <gtest/gtest.h>
#include <initializer_list>
#include <sstream>
#include <vector>

TEST(DataStructures, List) {
  std::vector<int> data{1, 2, 7, 12};
  ry::list<int> l(data);
  ASSERT_EQ(l.size(), 4);
  auto b = l.begin();
  ASSERT_EQ(*b, 1);
  decltype(l)::size_type i{0};
  std::for_each(l.begin(), l.end(), [&i, &data](int val) {
    ASSERT_EQ(val, data[i]);
    ++i;
  });

  std::ostringstream oss;
  oss << l;

  auto s = oss.str();
  ASSERT_EQ(s, "[1,2,7,12]");
}

TEST(DataStructures, AlignedAlloc) {
  std::size_t align = 64;
  auto int_ptr = ry::aligned_alloc<int>(align, 4 * sizeof(int));
  EXPECT_NE(int_ptr, nullptr);
  EXPECT_EQ(reinterpret_cast<std::uintptr_t>(int_ptr) % align, 0);
  ry::aligned_free(int_ptr);

  align = 512;
  auto dbl_ptr = ry::aligned_alloc<double>(align, 8 * sizeof(double));
  EXPECT_NE(dbl_ptr, nullptr);
  EXPECT_EQ(reinterpret_cast<std::uintptr_t>(dbl_ptr) % align, 0);
  ry::aligned_free(dbl_ptr);
}

TEST(DataStructures, Heap) {
  ry::heap<int> h({1, 2, 3});
  ASSERT_EQ(h.size(), 3);

  ry::heap<int> h_empty;
  ASSERT_EQ(h_empty.size(), 0);
}
