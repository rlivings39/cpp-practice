# Data flow analysis notes

A data flow analysis statically proves facts about a program or fragment. At a basic level facts are propagated around the CFG until a fixed-point is reached.

A join semilattice (abstract algebra) is a poset with where every two emelents have a least upper bound (called a **join**). So `join(a,b) >= a, join(a,b) >= b, join(a,a) = a`

For the problem of computing possible values of an integer variable use the lattice of sets of integers with set inclusion as the relation and set union as the join.

Lattices can be represented as Hasse diagrams. Finding the join amounts to finding the least common ancestor in the dag. [Efficient Implementation of Lattice Operations](https://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.106.4911) describes an efficient algorithm for this for program analysis.

We run into issues trying to compute possible value sets where the sets could grow too large. So we can cap the set cardinality at a value, say 3, and represent anything larger with the value `T` (pronounced "top") meaning that the value could be anything.

When a variable is uninitialized it has no possible values. This is represented as `⊥` (i.e. "bottom").

To propagate through a cfg, at each statement compute preconditions and postconditions on the variable of interest for its possible values. At control flow joins, the possible value sets are unioned.

Effects of statements are modeled by a **transfer function**. The transfer function takes 2 arguments: the statement and the state of the program prior to the statement. It returns the state after the statement has run.

Given a basic block with preconditions `in_i` we compute:

```
out = transfer(basic_block, join(in_1, in_2, ..., in_n))
```

to compute the postconditions of that block. Repeat this computation on all basic blocks until we reach a fixed-point.

The iteration is guaranteed to converge for a lattice of finite height and monotonic transfer function. At worst all values become top, but at least we converge.

The iteration can be optimized by only reprocessing basic blocks that had at least one of their inputs changed on the last iteration. This is typically done via a worklist queue. Using this the time complexity is `O(m * |L|)` where `m` is the number of basic blocks and `|L|` is the size of the lattice.



## References

* https://clang.llvm.org/docs/DataFlowAnalysisIntro.html
