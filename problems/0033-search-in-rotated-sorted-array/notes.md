## Approach

A sorted array rotated at an unknown pivot still has a useful property:
for any `mid`, at least one side of it is a clean, un-rotated sorted
run. Binary search works if, at each step, we figure out which side is
sorted and whether `target` lies within it.

Compare `nums[mid]` against the right endpoint `nums[right]` to locate
the sorted side:

- `nums[mid] > nums[right]` - the pivot is to the right, so
  `[left, mid]` is sorted ascending.
- otherwise `[mid, right]` is the sorted side.

Then check whether `target` falls inside the sorted side's value range.
If it does, move into that side; if not, the target must be in the
rotated remainder, so move the other way. Either way we discard the
half on the wrong side of `mid` (`left = mid + 1` or `right = mid - 1`),
halving the window each iteration.

## Complexity

- Time: `O(log n)`
- Space: `O(1)`.
