## Approach

Reverse the links in place with three pointers. Walk `curr` down the
list, and at each node stash its `next`, flip `curr->next` to point back
at `prev`, then slide both `prev` and `curr` forward.

Saving `next` before the flip is what keeps the traversal alive - once
`curr->next` is rewired we would otherwise lose the rest of the chain.
When `curr` falls off the end, `prev` is sitting on the old tail, which
is the new head.

The commented recursion is the same idea threading `(curr, prev)` through
the call stack instead of a loop.

## Complexity

- Time: `O(n)`
- Space: `O(1)` - becomes `O(n)` if we use recursion
