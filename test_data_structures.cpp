
#include "data_structures.hpp"
#include <algorithm>
#include <array>
#include <gtest/gtest.h>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <list>
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
