## Approach

**Backward merge using three pointers:**

- `i` and `j`: pointers to the last elements in `nums1` (up to `m - 1`) and `nums2` (up to `n - 1`)

- `cursor`: write position starting from the end of `nums1` at index `m + n - 1`

Compare elements from both arrays and place the larger one at `cursor`, moving the respective pointer backward. After `nums1` is exhausted, copy any remaining elements from `nums2`.

## Complexity

- Time: `O(m + n)`
- Space: `O(1)`
