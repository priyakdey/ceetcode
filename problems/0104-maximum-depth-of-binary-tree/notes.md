## Approach

The depth of a tree is `1` (for the root itself) plus the depth of its deeper subtree.
This definition is recursive, so the function asks the same question of each child and
combines the answers:

- `NULL` node → empty tree, depth `0`
- otherwise recurse into both children, take the larger depth, and add `1` for the
  current node

Because each call must resolve both children before it can return, this is a
post-order DFS — the work happens on the way back up as the depths bubble toward the
root.

## Complexity

- Time: `O(n)` - every node is visited exactly once.
- Space: `O(h)` - for the recursion stack, where `h` is the tree height `O(log n)`
  when balanced, `O(n)` when skewed.
