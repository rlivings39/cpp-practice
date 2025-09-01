# LeetCode practice notes

Started with Blind 75

* Binary tree problems have generally been good
* Things I've looked up
    * C++ checks for `isalphanumeric`, and std::filter
    * Default construction in maps/sets with `[]`
* Use iterators/pointers for checks

| Problem | Notes | Actions |
| -----   | ----- | ------  |
| 1. Two sum | Can use a single hash table. Do lookups while iterating  rather than 2 passes. |
| 141. Linked list cycle | 2 pointers (tortoise and hare) or visited set | |
| 268. Missing number | Can use closed form sum formula for fast algo | |
| 21. Merge 2 sorted lists | do-while loop can be useful | |
| 572. Subtree of another tree | Visit main tree and match subtree at  | |each node
| 190. Reverse uint32_t bits | | TODO make faster |
| 206. Reverse Linked List | Prev, head, next pointers that we advance | |
| 121. Best Time to Buy and Sell Stock | [Dynamic programming solution](leetcode.cpp) keeps running score and min which easily extends to next problem |  |
| 128. Longest Consecutive Sequence | Dunno | Solve it |
| 3. Longest Substring Without Repeating Characters | Solved but slow | Speed up and analyze technique |
| 70. Climbing Stairs | DP problem consider n-2, n-1. Iterative Fibonacci |  |
| 191. Number of 1 Bits | | Revisit |
| 133. Clone Graph | DFS/BFS traversal | Revisit |
| 128. Longest Consecutive Sequence | Slow | Try union-find? Research |
| 5. Longest Palindromic Substring | 2 pointer solution pretty good | Manacher's algorithm w/ DP is `O(n)` |
| 516. Longest Palindromic Subsequence | DP solution with caching works well. Took me a *very* long time | Practice dp more |
| 11. Container With Most Water | 2 pointer greedy solution | Figure out why 2 pointer solution is maximal |
| 15. 3Sum | Got solution without uniqueness | Read up on and implemented 2 pointer solution + sort |
| 2. Add Two Numbers | Can do a single loop by checking each list for `nullptr` | |
| 647. Palindromic Substrings | Brute force is each substring w/ 2 pointers. Optimize with DP/caching for each window. Check endpoints, check cache for substring| |

## Dynamic programming

A problem must have **optimal substructure** and **overlapping sub-problems** for dynamic programming to be relevant.

If a problem can be solved by combining optimal solutions to non-overlapping sub-problems, this is called **divide and conquer**.

**Optimal substructure** means that the solution to a given optimization problem can be obtained from the combination of optimal solutions to sub problems, often by means of recursion.

**Overlapping sub-problems** means that the space of sub-problems must be small so that any recursive algorithm solving the problem should solve the same sub-problems over and over rather than generating new sub-problems. Dynamic programming solves each sub-problem only once:

* **Top-down** approach: The direct fallout of recursive formulation. If a solution can be formed by recursively using solutions to sub-problems, one can memoize solutions to the sub-problems
* **Bottom-up** approach: We can try to solve the sub-problems first and then use their solutions to build-on and arrive at solutions to bigger problems. Solve the problems in increasing size (or some other ordering). Store the solutions to the "smaller" problems and refer to them when constructing the solutions to the larger problems.

Strategy:

1. Characterize structure of optimal solution
2. Recursively compute optimal solution based on solutions to subproblems.
3. Compute the value of an optimal solution (recurse and memoize or perform iteratively). Subproblems need to be computed first.
4. Construct optimal solution from computed information (optional). E.g. construct longest subsequence rather than just the length

Complexity of DP is `# subproblems X to solve each subproblem given that lookup is O(1)`

### Dynamic programming in detail

Optimal substructure and overlapping subproblems are the two attributes for when dynamic programming can apply to an optimization problem.

Optimal substructure means that you can construct an optimal solution from optimal solutions to subproblems.

CLRS presents a common pattern

1. Show that a solution involves making a choice that creates subproblems (e.g. where to split)
2. Suppose you have a choice leading to an optimal solution (though you don't yet have it)
3. Given this choice, determine the subproblems that arise and how to characterize the space of subproblems
4. Show that solutions to the subproblems must also be optimal via contradiction. Suppose one isn't optimal. Then show that if you replaced the suboptimal solution with an optimal solution that the global solution improves.

Optimal substructure varies in how many subproblems an optimal solution uses and how many choices you have in determining which subproblems are used.

Subproblems must also be **independent** of each other.

Subproblems must also overlap for dynamic programming to be useful. If they don't overlap then you're often just in divide and conquer land. Having the problems overlap means that you can leverage the repeated problems to make your algorithm more efficient either by memoizing in a top-down solution or building up solutions in a bottom-up solution.

When all subproblems must be solved, a bottom-up solution often has better constant factors than a top-down one with memoization. When only some of the subproblems must be solved, top-down with memoization can avoid solving the extraneous ones.

### Leetcode TODOs

* [ ] Dynamic programming from CLRS book
* [ ] Divide and conquer from CLRS book
* [ ] Graph algos from CLRS book
* [ ] Binary representation questions. I have no intuition here.
* [ ] Do list reversal again
* [ ] Iterative solutions for tree traversals?
* [ ] Learn dynamic programming and memoization
* [ ] Make sure to write your base cases!!!
