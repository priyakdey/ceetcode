## Approach

**DP tracking both min and max** products ending at each position. A negative number can flip signs, turning a small/negative product into a large one, so we must track both extremes.

At each element, choose the best of three: extend the previous min product, extend the previous max product, or start fresh at the current element.

Special case: reset to 1 (neutral multiplier) on encountering 0, but consider 0 itself as a candidate for the overall maximum.

## Complexity

- Time: `O(n)`
- Space: `O(1)`
