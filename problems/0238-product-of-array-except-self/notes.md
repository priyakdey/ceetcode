## Approach

Two-pointer single-pass technique: use two pointers moving toward each other to compute prefix and suffix products in one loop.

Algorithm:
1. Initialize result array to all 1s
2. Use `left` pointer starting at 1 (accumulates products from elements to the left)
3. Use `right` pointer starting at `numsSize - 2` (accumulates products from elements to the right)
4. In a single loop that runs until pointers meet or cross:
   - Multiply prefix products into `result[left]` going left-to-right, then increment `left`
   - Multiply suffix products into `result[right]` going right-to-left, then decrement `right`

Each position ends up with the product of all elements except itself, computed simultaneously from both directions in one pass.

## Complexity

- Time: `O(n)`
- Space: `O(1)`
