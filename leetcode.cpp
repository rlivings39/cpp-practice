#include <algorithm>
#include <format>
#include <gtest/gtest.h>
#include <limits>
#include <ranges>
#include <sstream>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_set>
#include <utility>

using namespace std::literals;
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

TEST(LeetCode, DynamicProgrammingBuySellStock) {
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
  std::string s = "adsfadf";
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

TEST(LeetCode, CloneGraphDfs) {
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

bool wordBreak(std::string s, size_t start, std::vector<std::string> &wordDict,
               std::unordered_set<std::string> &word_set) {
  // 1. Brute force: From start look for a viable word of all given lengths
  if (start == s.size()) {
    return true;
  }
  for (size_t len{1}; len < s.size() - start + 1; ++len) {
    auto prefix_s = s.substr(start, len);
    if (word_set.count(prefix_s) > 0 &&
        wordBreak(s, start + len, wordDict, word_set)) {
      return true;
    }
  }
  return false;
}

bool wordBreak(std::string s, std::vector<std::string> &wordDict) {
  std::unordered_set<std::string> word_set(wordDict.begin(), wordDict.end());
  return wordBreak(s, 0, wordDict, word_set);
}

TEST(LeetCode, WordBreak) {
  std::string s{"leetcode"};
  std::vector<std::string> word_dict{"leet", "code"};

  ASSERT_TRUE(wordBreak(s, word_dict));

  s = "catsandog";
  word_dict = {"cats", "dog", "sand", "and", "cat"};
  ASSERT_FALSE(wordBreak(s, word_dict));
}

// TODO: caching uses way too much memory
// TODO: Can this be iterative or at least bottom-up to make the cache more
// efficient?
std::pair<int, std::string>
doCache(size_t i, std::pair<size_t, std::string> res,
        std::vector<std::pair<size_t, std::string>> &cache) {
  cache[i] = res;
  return res;
}

std::pair<int, std::string> longestPalindromeSubsequence(
    std::string_view s, size_t n, size_t i, size_t j,
    std::vector<std::pair<size_t, std::string>> &cache) {
  auto flati = i * n + j;
  if (s.empty()) {
    return doCache(flati, {0, ""}, cache);
  }
  if (s.size() == 1) {
    return doCache(flati, {1, std::string(s.substr(0, 1))}, cache);
  }
  if (cache[flati].first != 0) {
    return cache[flati];
  }
  int res_len{1};
  std::string res_str(s.substr(0, 1));
  auto slen = s.size();
  // For every i,j compute the longest subsequence in the range s[i], ..., s[j]
  // using a recurrence
  if (s.front() == s.back()) {
    // We can extend the palindrome in the inner segment
    if (slen == 2) {
      return doCache(flati, {2, std::string(s)}, cache);
    }
    auto sub_res = longestPalindromeSubsequence(s.substr(1, slen - 2), n, i + 1,
                                                j - 1, cache);
    res_len = 2 + sub_res.first;
    res_str = s.front() + sub_res.second + s.back();
  } else {
    // We can't extend so just choose the larger of s[i],...,s[j-1] and
    // s[i+1],...s[j]
    auto sub_res1 =
        longestPalindromeSubsequence(s.substr(0, slen - 1), n, i, j - 1, cache);
    auto sub_res2 =
        longestPalindromeSubsequence(s.substr(1, slen - 1), n, i + 1, j, cache);
    std::pair<int, std::string> *sub_res;
    if (sub_res1.first >= sub_res2.first) {
      sub_res = &sub_res1;
    } else {
      sub_res = &sub_res2;
    }
    res_len = sub_res->first;
    res_str = sub_res->second;
  }
  return doCache(flati, {res_len, res_str}, cache);
}
// Given a string find the longest palindromic subsequence (can skip letters)
std::pair<int, std::string> longestPalindromeSubsequence(std::string s) {
  std::vector<std::pair<size_t, std::string>> cache(s.size() * s.size(),
                                                    std::make_pair(0, ""s));
  // std::vector<std::pair<size_t, std::string>> cache(1,
  // std::make_pair(0,""s));
  return longestPalindromeSubsequence(std::string_view(s), s.size(), 0,
                                      s.size() - 1, cache);
}

int longestPalindromeSubseq(std::string s) {
  return longestPalindromeSubsequence(s).first;
}

TEST(LeetCode, Longestpalindromesubsequence) {
  ASSERT_EQ(longestPalindromeSubsequence("a"s), std::make_pair(1, "a"s));
  ASSERT_EQ(longestPalindromeSubsequence("aa"s), std::make_pair(2, "aa"s));
  ASSERT_EQ(longestPalindromeSubsequence("aaaa"s), std::make_pair(4, "aaaa"s));
  ASSERT_EQ(longestPalindromeSubsequence("aca"s), std::make_pair(3, "aca"s));
  ASSERT_EQ(longestPalindromeSubsequence("abcdbea"s),
            std::make_pair(5, "abcba"s));

  ASSERT_EQ(
      longestPalindromeSubsequence(
          "euazbipzncptldueeuechubrcourfpftcebikrxhybkymimgvldiwqvkszfycvqyvtiw"
          "fckexmowcxztkfyzqovbtmzpxojfofbvwnncajvrvdbvjhcrameamcfmcoxryjukhplj"
          "wszknhiypvyskmsujkuggpztltpgoczafmfelahqwjbhxtjmebnymdyxoeodqmvkxitt"
          "xjnlltmoobsgzdfhismogqfpfhvqnxeuosjqqalvwhsidgiavcatjjgeztrjuoixxxoz"
          "nklcxolgpuktirmduxdywwlbikaqkqajzbsjvdgjcnbtfksqhquiwnwflkldgdrqrnwm"
          "shdpykicozfowmumzeuznolmgjlltypyufpzjpuvucmesnnrwppheizkapovoloneaxp"
          "finaontwtdqsdvzmqlgkdxlbeguackbdkftzbnynmcejtwudocemcfnuzbttcoew"),
      std::make_pair(
          159,
          "ebzncduecbcuebxkglqvsqtifeoovzpwnnemcupjzypyjglozemmoodmnllfiqhqnsjqqalwditgzoxxxozgtidwlaqqjsnqhqifllnmdoommezolgjypyzjpucmennwpzvooefitqsvqlgkxbeucbceudcnzbe"s));
}

/**
 * Rod cutting problem from CLR section 14.1
 *
 * Given a rod of length n and a table p of prices
 * where p[i] is the price of a rod of length i, determine
 * how to cut the rod into 0-n pieces to maximize price. Return
 * the optimal price.
 */

int cutRodTopdownWorker(std::vector<int> &p, std::vector<int> &cache, int n) {
  int res{0};
  if (cache[n] >= 0) {
    return cache[n];
  }
  if (n == 0) {
    res = 0;
  }
  for (int i : std::ranges::iota_view(1, n + 1)) {
    res = std::max(res, p[i] + cutRodTopdownWorker(p, cache, n - i));
  }
  cache[n] = res;
  return res;
}

int cutRodTopdown(std::vector<int> &p, int n) {
  std::vector<int> cache(n + 1, -1);
  return cutRodTopdownWorker(p, cache, n);
}

int cutRodBottomUp(std::vector<int> &p, int n) {
  std::vector<int> cache(n + 1, -1);
  cache[0] = 0;
  // Loop over rod lengths from small to large. These are the
  // small subproblems.
  for (int len : std::ranges::iota_view(1, n + 1)) {
    int res{0};
    // Evaluate the price of a cut at each location on the smaller rod
    for (int pos : std::ranges::iota_view(1, len + 1)) {
      res = std::max(res, p[pos] + cache[len - pos]);
    }
    cache[len] = res;
  }

  return cache[n];
}

TEST(LeetCode, CutRod) {
  std::vector<int> p{0, 1, 5, 8, 9, 10, 17, 17, 20, 24, 30};
  EXPECT_EQ(cutRodTopdown(p, 4), 10);
  EXPECT_EQ(cutRodTopdown(p, 10), 30);

  EXPECT_EQ(cutRodBottomUp(p, 4), 10);
  EXPECT_EQ(cutRodBottomUp(p, 10), 30);
}

// Matrix-chain multiplication from CLRS section 14.2.
//
// Given a sequence `<A1, A2, ..., An>` of n arbitrarily but compatibly shaped
// matrices, parenthesize the product A1*A2*...*An in a way that minimizes the
// number of scalar multiplications. Assume that the size of matrix Ai is pi-1 x
// pi. Take <p0,p1,...,pn> as a parameter.
//
// The number of ways to parenthesize is the sequence of Catalan numbers that
// grows with a lower bound \Omega(4^n / n^(3/2))
//
// The runtime of the computation can be changed significantly based on how the
// multiplication is parenthesized.
//
// 1. Suppose you compute the product Ai*...*Aj for i <= j. A step to associate
// this is to choose a k, i<=k<j, and split the product into 2.
// (Ai*...*Ak)*(Ak+1*...*Aj).
//
// In an optimal solution, each of these subproblems must use an optimal
// solution. Otherwise substituting that solution would yield a better
// algorithm.
//
// 2. Once you have your subproblem solutions, to combine to a larger one you
// just add the cost of the two subproblems and the cost of multiplying the
// subproblem result matrices to get the new result.

template <typename T> struct matrix {
  explicit matrix(size_t nrows, size_t ncols)
      : fNrows(nrows), fNcols(ncols), fData(fNcols * fNrows) {}
  T &operator()(size_t i, size_t j) {
    return this->fData[i * fNrows + j];
  }

  const T &operator()(size_t i, size_t j) const {
    return this->fData[i * fNrows + j];
  }

  size_t numel() {
    return fNrows * fNcols;
  }

  void fill(T val) {
    std::fill(fData.begin(), fData.end(), val);
  }

private:
  size_t fNrows;
  size_t fNcols;
  std::vector<T> fData;
};

auto matrixChainBottomUp(std::vector<int> const &p) {
  int n = p.size() - 1;
  matrix<int> costs(n, n), splits(n, n);

  // Initialize base cases for window of size 0
  for (int i : std::ranges::iota_view(0, n)) {
    costs(i, i + 1) = 0;
  }
  splits.fill(-1);

  // Now compute the rest
  for (int len : std::ranges::iota_view(1, n + 1)) {
    for (int i : std::ranges::iota_view(0, n - len)) {
      int j = i + len;
      int res{std::numeric_limits<int>::max()};
      int split{0};
      // Check each feasible split
      for (int k : std::ranges::iota_view(i, j)) {
        int current_cost =
            costs(i, k) + costs(k + 1, j) + p[i] * p[k + 1] * p[j + 1];
        if (current_cost < res) {
          res = current_cost;
          split = k;
        }
      }
      costs(i, j) = res;
      splits(i, j) = split;
    }
  }
  return std::make_pair(costs(0, n - 1), splits);
}

void printParensWorker(matrix<int> const &splits, int i, int j,
                       std::ostringstream &oss) {
  if (i == j) {
    oss << std::format("A{}", i);
  } else {
    oss << "(";
    printParensWorker(splits, i, splits(i, j), oss);
    printParensWorker(splits, splits(i, j) + 1, j, oss);
    oss << ")";
  }
}

std::string printParens(matrix<int> const &splits, int i, int j) {
  std::ostringstream oss;
  printParensWorker(splits, i, j, oss);
  return oss.str();
}

TEST(LeetCode, MatrixChainMultiplication) {
  std::vector<int> prob1{10, 100, 5, 50};
  auto [cost, splits] = matrixChainBottomUp(prob1);
  EXPECT_EQ(cost, 7500);

  std::vector<int> prob2{30, 35, 15, 5, 10, 20, 25};
  std::tie(cost, splits) = matrixChainBottomUp(prob2);
  EXPECT_EQ(cost, 15125);
  EXPECT_EQ(printParens(splits, 0, prob2.size() - 2), "((A0(A1A2))((A3A4)A5))");
}

// 3 sum - given a vector of numbers find unique triples that sum to zero where
// indices are unique
std::vector<std::vector<int>> threeSum(std::vector<int> &nums) {
  std::sort(nums.begin(), nums.end());
  std::vector<std::vector<int>> res;
  if (nums.size() < 3) {
    return res;
  };
  size_t start{0}, left{0}, right{0};
  for (auto val : nums) {
    // Skip over duplicate start values
    if (start > 0 && nums[start - 1] == val) {
      ++start;
      continue;
    }
    left = start + 1;
    right = nums.size() - 1;
    while (left < right) {
      auto left_val = nums[left];
      auto right_val = nums[right];
      auto sum2 = left_val + right_val;
      if (sum2 == -val) {
        res.push_back({val, left_val, right_val});
        ++left;
        // Skip over duplicate middle values
        while (left < right && nums[left] == nums[left - 1]) {
          ++left;
        }
      } else if (sum2 < -val) {
        ++left;
      } else {
        --right;
      }
    }
    ++start;
  }
  return res;
}

TEST(LeetCode, ThreeSum) {
  std::vector<int> nums{2,  -3, 0, -2, -5, -5, -4, 1,  2,
                        -2, 2,  0, 2,  -4, 5,  5,  -10};
  auto res = threeSum(nums);
  std::vector<std::vector<int>> expected = {
      {-10, 5, 5}, {-5, 0, 5}, {-4, 2, 2}, {-3, -2, 5}, {-3, 1, 2}, {-2, 0, 2}};
  EXPECT_EQ(res, expected);
}
