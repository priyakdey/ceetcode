## Approach

We first sort the array (ascending or descending does not matter - it only reverses the
iteration flow). Sorting reduces the problem from a three-element search to a two-element one:
fix one element `nums[i]`, and the task becomes finding a pair in the remaining elements that
sums to `-nums[i]`.

For each fixed `i`, we run two pointers over the subarray to its right - `left` at `i + 1` and
`right` at the end. Because the array is sorted, the sum tells us which way to move: if it is
too small we advance `left` to gain value, if too large we retreat `right` to lose value, and
when it hits the target we record the triplet. To keep the triplets unique, we skip over
duplicate values for both the fixed element and the pointers.

## Complexity

- Time: `O(n log(n)) + O(n ^ 2)`
- Space: `O(n)`
