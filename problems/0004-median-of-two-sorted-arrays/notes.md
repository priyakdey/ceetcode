## Approach

Two-pointer merge: advance through both sorted arrays, picking the
smaller head each step, until `(n + m) / 2` elements have been
consumed. No merged buffer - only track the last two values stepped
over (`prevElement`, `midElement`). When one array is exhausted before
the midpoint, drain the other.

For odd total length, return `midElement`. For even, return the
average of `prevElement` and `midElement`.

A pre-step seeds `midElement` with the smaller of the two heads so
the first iteration has a valid `prevElement` to advance from.

> The classic `O(log(min(n, m)))` binary-search solution is faster
> but not what's implemented here.

## Complexity

- Time: `O(n + m)` - at most `(n + m) / 2` steps, each `O(1)`.
- Space: `O(1)` - two running ints plus indices.
