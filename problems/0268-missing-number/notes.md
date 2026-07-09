## Approach

The array holds `n` distinct numbers drawn from `0..n`, so exactly one is missing. The
full range `0..n` sums to `n(n+1)/2` (Gauss); subtract every element actually present and
what's left is the missing number. One pass, no extra space, no overflow worries for
LeetCode-sized `n`.

## Complexity

- Time: `O(n)`
- Space: `O(1)`
