## Approach

Walk two pointers at different speeds - `slow` one node at a time, `fast`
two at a time. If the list ends, there is no cycle and `fast` (or
`fast->next`) falls off into `NULL`. If there *is* a cycle, `fast` keeps
lapping the loop and eventually laps `slow` from behind, so the two must
collide on the same node.

The intuition: once both pointers are inside the loop, `fast` closes the
gap to `slow` by exactly one node every step, so the distance between them
strictly shrinks to zero - they can never step over each other. A meeting
is therefore guaranteed whenever a cycle exists.

This is called **[Floyd's Cycle Algorithm or Tortoise And Hare algorithm](https://cp-algorithms.com/others/tortoise_and_hare.html)** -
visit for more details.

The loop guard checks `fast != NULL && fast->next != NULL` before the
double step, which safely handles both empty and odd-length lists.

## Complexity

- Time: `O(n)`
- Space: `O(1)`
