## Approach

Use a **min-heap** (binary-heap priority queue) keyed on node value.
Seed it with the head of every non-empty list, then repeatedly pop the
smallest node, append it to the result chain, and push that node's
`next` back in. The heap always surfaces the global minimum across all
lists, so the output comes out fully sorted.

The heap is backed by a **fixed array of capacity `k`** (the number of
lists) with a `size` cursor - no dynamic resizing needed. The invariant
is that at most one node per list is in the heap at any time: each pop
removes one node and pushes at most one (its successor), so `size` never
exceeds `k`. Parent/child links are pure index math
(`parent = (i-1)/2`, `children = 2i+1`, `2i+2`); `push` sifts up,
`pop` swaps the last element to the root and sifts down.

A dummy head collects the spliced nodes so the first append needs no
special case. Empty (`NULL`) list heads are skipped on seeding, and
`dummy->next` is pre-set to `NULL` so all-empty input returns cleanly.

Nodes are **relinked in place** - no new list nodes are allocated,
only the `k`-slot heap array and the dummy head.

## Complexity

- Time: `O(n log k)` - `n` total nodes, each pushed/popped once at
  `O(log k)` per heap op.
- Space: `O(k)` - the heap holds at most `k` nodes.
