## Approach

Merge **in place** by splicing nodes - no new allocation, no dummy
head. Pick whichever list has the smaller first value as the result
`head`, and keep a `prev` pointer to the last node already stitched
into the merged chain.

Walk two pointers, `curr1` along the chain `prev` belongs to and
`curr2` along the other list:
- If `curr1`'s value is smaller, it's already in place - just advance
  `prev` and `curr1`.
- Otherwise `curr2` is smaller, so splice it in after `prev`
  (`prev->next = curr2`, `curr2->next = curr1`), then advance `prev`
  to it and step `curr2` forward.

When either list runs out, any remaining tail is already sorted and
already linked - except a leftover `curr2`, which gets appended once.

Picking the smaller head up front is what lets us reuse `list1`'s
spine as the backbone and only ever move `list2`'s nodes into it.

## Complexity

- Time: `O(n + m)`
- Space: `O(1)`
