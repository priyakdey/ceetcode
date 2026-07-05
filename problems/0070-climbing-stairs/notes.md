## Approach

The last move to reach step `n` is either a `+1` from `n-1` or a `+2` from `n-2`,
so `ways(n) = ways(n-1) + ways(n-2)` — the Fibonacci recurrence, with base cases
`ways(1) = 1` and `ways(2) = 2`. Computing bottom-up with two rolling variables
avoids recursion and keeps space constant.

## Complexity

- Time: `O(n)`
- Space: `O(1)`

