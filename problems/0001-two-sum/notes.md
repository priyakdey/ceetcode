## Approach

The brute force approach checks every pair in `O(n^2)` - but we can do better. Using a **hashmap**,
we store each element in the array we visit during traversal. This allows us to answer the question
if we have already encountered the compliment of the current element `target - nums[current_index]` 
in an `amortized O(1)` time. If it does, we have found our pair.


## Complexity

- Time: `O(n)` - single pass over `nums` with amortized `O(1)` hashmap
  lookups.
- Space: `O(n)` - the hashmap holds up to `n - 1` entries before the
  match is found.
