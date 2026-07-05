## Approach

Two trees are the same when their roots match **and** their left and right subtrees
are pairwise the same. This definition is recursive, so the function calls itself on
the child pairs: for the current pair of nodes, check they agree, then delegate the
same question to `(p->left, q->left)` and `(p->right, q->right)`.

Each call resolves in order:

- both `NULL` → this branch is same, return `true`
- exactly one `NULL`, or values differ → structure/value mismatch, return `false`
- otherwise recurse into both children; both must be same

The `&&` short-circuits, so the first mismatch anywhere stops the descent. Since the
recursion follows each path down before backtracking, this is implicitly a DFS
traversal of the two trees in lockstep.

## Complexity

- Time: `O(n)` -  where `n` is the number of nodes in the smaller tree — every node is
  visited at most once, and a mismatch returns early.
- Space: `O(h)` -  for the recursion stack, where `h` is the tree height `O(log n)`
  when balanced, `O(n)` when skewed.
