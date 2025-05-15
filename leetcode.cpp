#include <gtest/gtest.h>
#include <stack>
#include <unordered_set>

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

class Node {
public:
  int val;
  std::vector<Node *> neighbors;
  Node() {
    val = 0;
    neighbors = std::vector<Node *>();
  }
  Node(int _val) {
    val = _val;
    neighbors = std::vector<Node *>();
  }
  Node(int _val, std::vector<Node *> _neighbors) {
    val = _val;
    neighbors = _neighbors;
  }
};

Node *cloneGraph(Node *node) {
  // DFS
  std::stack<Node *> workStack;
  std::unordered_map<Node *, Node *> copies;
  if (node == nullptr) {
    return node;
  }
  Node *result = new Node(node->val);
  workStack.push(node);
  copies[node] = result;
  while (!workStack.empty()) {
    auto oldNode = workStack.top();
    auto newNode = copies[oldNode];
    workStack.pop();
    for (auto neighbor : oldNode->neighbors) {
      auto newNeighborIt = copies.find(neighbor);
      if (newNeighborIt == copies.end()) {
        newNeighborIt =
            copies.insert({neighbor, new Node(neighbor->val)}).first;
        workStack.push(neighbor);
      }
      newNode->neighbors.push_back(newNeighborIt->second);
    }
  }

  return result;
}

TEST(Leetcode, CloneGraphDfs) {
  Node n1(1), n2(2), n3(3), n4(4);
  n2.neighbors.push_back(&n4);
  n4.neighbors.push_back(&n2);
  n1.neighbors.push_back(&n3);
  n3.neighbors.push_back(&n1);
  auto res = cloneGraph(&n1);
  ASSERT_EQ(res->neighbors.size(), 1);
  ASSERT_EQ(res->val, 1);
  ASSERT_EQ(res->neighbors[0]->val, 3);
}
