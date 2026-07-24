## Approach

Reordering interleaves the head and tail of the list: `[1,2,3,4,5]` becomes
`[1,5,2,4,3]`. The algorithm reconstructs it in three phases:

1. **Find the midpoint** using slow/fast pointers: slow steps one node per iteration,
   fast steps two. When fast reaches the end, slow is at the middle.

2. **Split and reverse** the second half: cut the list at the midpoint and reverse the
   tail. This moves elements from the end into a reachable order.

3. **Merge by interleaving**: walk both halves in parallel, splicing nodes from the
   second half into gaps in the first. Stash next pointers before reassigning to avoid
   losing track of unvisited nodes.

Each phase is linear, so the total cost is three passes over the list.

## Complexity

- Time: `O(n)`
- Space: `O(1)`
