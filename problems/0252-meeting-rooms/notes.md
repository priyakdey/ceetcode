## Approach

A person can attend every meeting only if no two intervals overlap. Sort the intervals
by start time, then a single pass is enough: once meetings are ordered, an overlap can
only happen between neighbours, so we just check each meeting against the one before it.

## Complexity

- Time: `O(n log n)`
- Space: `O(1)`
