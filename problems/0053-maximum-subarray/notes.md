## Approach

[Kadane's algorithm](https://en.wikipedia.org/wiki/Maximum_subarray_problem#Kadane's_algorithm).
Track `currSum`, the best subarray sum *ending at* the current element.
At each step there are only two candidates: extend the previous run by
adding `nums[i]`, or start a fresh run at `nums[i]` - take whichever is
larger. A run is abandoned exactly when its trailing sum has gone
negative, since dropping it can only help.

Keep a running `maxSum` of the best `currSum` seen so far; that is the
answer. Seeding both from `nums[0]` handles the all-negative case,
where the answer is simply the largest single element.

## Complexity

- Time: `O(n)`
- Space: `O(1)`

