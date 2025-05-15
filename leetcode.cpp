#include <gtest/gtest.h>

namespace ry {
// 121. Best Time to Buy and Sell Stock
// https://leetcode.com/problems/best-time-to-buy-and-sell-stock/?envType=problem-list-v2&envId=oizxjoit
int maxProfit(std::vector<int> prices) {
  if (prices.size() < 2) {
    return 0;
  }
  int score{0}, curr_min{prices[0]};
  for (size_t idx{1}; idx < prices.size(); ++idx) {
    score = std::max(score, prices[idx] - curr_min);
    curr_min = std::min(curr_min, prices[idx]);
  }

  return score;
}

} // namespace ry

TEST(Leetcode, DynamicProgrammingBuySellStock) {
  std::vector<int> prices{7, 6, 4, 3, 1};
  ASSERT_EQ(ry::maxProfit({7, 6, 4, 3, 1}), 0);
  ASSERT_EQ(ry::maxProfit({7, 1, 5, 3, 6, 4}), 5);
}
