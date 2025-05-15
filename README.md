# cpp-practice

Some exercises for me to shake off the C++ cobwebs. Generally build with CMake

```bash
cmake --build ./build --config Debug --target all
```
and run the tests in `./build/cpp-practice-test` to see if things are working right.

The unit test file [tests.cpp](tests.cpp) is a great place to start reading. It covers a matrix multiplication algorithm as well as review on modern C++ features.

[test_graph.cpp](test_graph.cpp) has tests for some graph algorithms

## Modern C++ features to review

Topics to cover:

* [x] Smart pointers, especially weak_ptr
* [x] r-value references, universal references
* [x] Private, virtual, etc. inheritance
* [x] Rule of 0,3,5 https://en.cppreference.com/w/cpp/language/rule_of_three
* [ ] Do some operator overloading
* [ ] Threading and parallelism, thread pools, condition vars, atomics
    * Useful https://stackoverflow.com/questions/3513045/conditional-variable-vs-semaphore
* [ ] Locks, mutexes, semaphores
* [x] Optional and variant
* [x] Lambdas w/ captures and init-capture `[x = std::move(obj)] { use(x); }`
* [x] Using override, default, delete, const on methods
* [x] C'tors, assignment, d'tors (copy and move)
* [ ] auto, decltype auto, std::decay, etc.
* [ ] Initializer lists, uniform initialization
* [ ] Argument packs/variadiac templates, fold expressions. Make a tuple
    * https://en.cppreference.com/w/cpp/language/pack has a lot of good detail. Crazy complicated.
    * https://en.cppreference.com/w/cpp/language/fold
* [ ] Smart pointers in graphs/cycles
* [ ] Basic ranges
* [ ] ADL
* [ ] Variable templates
* [ ] Generic lambdas
* [ ] if-init: `if(auto it = map.find(key); it != map.end()) { use(it); }`
* [ ] Structured binding declarations
* [ ] Inline variables for single definition of globals and class statics in headers
* [ ] `if constexpr`
* [ ] std::filesystem
* [ ] `<string_view>, <any>, <optional>, <variant>, <memory_resource>`
* C++ 20
* [ ] concepts
* [ ] ranges
* [ ] coroutines
* [ ] constexpr and consteval

General standard version references

* https://en.cppreference.com/w/cpp/11
* https://en.cppreference.com/w/cpp/14
* https://en.cppreference.com/w/cpp/17
* https://en.cppreference.com/w/cpp/20

## Leetcode practice notes

Started with Blind 75

* Binary tree problems have generally been good
* Things I've looked up
    * C++ checks for isalphanumeric, and std::filter
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
| 70. Climbing Stairs | | Iterative fibo and figure out why fibo is answer |

## Dynamic programming

A problem must have **optimial substructure** and **overlapping sub-problems** for dynamic programming to be relevant.

If a problem can be solved by combining optimal solutions to non-overlapping sub-problems, this is called **divide and conquer**.

**Optimal substructure** means that the solution to a given optimization problem can be obtained from the combination of optimal solutions to sub problems, often by means of recursion.

**Overlapping sub-problems** means that the space of sub-problems must be small so that any recursive algorithm solving the problem should solve the same sub-problems over and over rather than generating new sub-problems. Dynamic programming solves each sub-problem only once:

* Top-down approach: The direct fallout of recursive formulation. If a solution can be formed by recursively using solutions to sub-problems, one can memoize solutions to the sub-problems
* Bottom-up approach: We can try to solve the sub-problems first and then use their solutions to build-on and arrive at solutions to bigger problems. This can be tabular by iteratively generating solutions to bigger and bigger sub-problems.

### Leetcode TODOs

* [ ] Binary representation questions. I have no intuition here.
* [ ] Do list reversal again
* [ ] Iterative solutions for tree traversals?
* [ ] Learn dynamic programming and moization

