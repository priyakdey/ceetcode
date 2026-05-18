## Approach

Expand around every possible center. A palindrome is defined by its
center, and there are `2n - 1` of them: `n` single-character centers
(odd length) and `n - 1` between-character centers (even length).

For each index `i`:

- Odd case: start with `l = i - 1`, `r = i + 1` and walk outward
  while `s[l] == s[r]`.
- Even case: only run if `s[i] == s[i + 1]`; start with `l = i - 1`,
  `r = i + 2` and walk outward the same way.

After each expansion, the palindrome spans `[l + 1, r - 1]` with
length `r - (l + 1)`. Track the best span seen, then copy that slice
into a fresh malloc'd buffer for the return value.

## Complexity

- Time: `O(n^2)` - `O(n)` centers, each expanding up to `O(n)`.
- Space: `O(1)` - working memory; output buffer is `O(k)` where `k` is
  the result length.
