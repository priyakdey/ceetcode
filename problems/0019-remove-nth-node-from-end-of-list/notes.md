## Approach

Two pointers, one pass. Advance a `fast` pointer `n` nodes ahead of
`slow`, then walk both in lockstep until `fast` falls off the end. The
gap between them stays fixed at `n`, so when `fast` is `NULL`, `slow`
sits exactly one node *before* the target - ready to unlink it.

A **dummy head** pointing at `head` is the key trick: it gives `slow` a
valid node to stand on even when the target is the head itself. Without
it, removing the first node would need a special case. With it, the
removal collapses to a single uniform `slow->next = slow->next->next`,
and the answer is always `dummy.next`.

The dummy lives on the stack - no allocation, so nothing to `free`.

## Complexity

- Time: `O(n)`
- Space: `O(1)`
