## Approach

Inverting a tree means mirroring it: at every node the left and right subtrees swap
places. This definition is recursive — a node is inverted once both of its subtrees
are themselves inverted and then swapped — so the function delegates the same question
to each child before wiring the results back in:

- `NULL` node → empty tree, nothing to invert, return it as-is
- otherwise recurse into both children, then assign the inverted right subtree to
  `left` and the inverted left subtree to `right`

The two recursive calls happen before the swap, so the inversion completes from the
leaves upward — a post-order DFS where each node's work is done only after its
descendants are already mirrored. (Grabbing `l` and `r` into locals first also avoids
clobbering a pointer before it is read.)

## Complexity

- Time: `O(n)`
- Space: `O(h)` - for the recursion stack, where `h` is the tree height `O(log n)`
  when balanced, `O(n)` when skewed.
