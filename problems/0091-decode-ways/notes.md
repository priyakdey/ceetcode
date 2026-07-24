## Approach

**Top-down dynamic programming** with memoization. At each position, we have two choices:
- Decode a **single digit** (1–9); skip if it's '0'.
- Decode **two digits** (10–26); valid only if the digit is '1' (any second digit) or '2' followed by 0–6.

Recurse from each choice, caching results by position to avoid redundant work. Base case: reaching the end (all digits consumed) counts as one valid decoding.

## Complexity

- Time: `O(n)` - each position memoized once, 2 choices per state.
- Space: `O(n)` - cache of size `n`, recursion depth `O(n)`.
