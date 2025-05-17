#include <gtest/gtest.h>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_set>

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

TEST(Leetcode, WordBreak) {
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

TEST(Leetcode, Longestpalindromesubsequence) {
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
