## Approach

**Binary search** exploiting the property that both halves of a rotated sorted array remain sorted, but the **minimum always lies in the right half** (the one with smaller values).

Compare `mid` with the last element. If `mid <= nums[last]`, mid is in the right sorted half—update the min candidate and search left (shrink right). If `mid > nums[last]`, mid is in the left sorted half—skip it and search right (move left forward).

This naturally handles multiple rotations or no rotation: the invariant holds regardless.

## Complexity

- Time: `O(log n)`
- Space: `O(1)`
