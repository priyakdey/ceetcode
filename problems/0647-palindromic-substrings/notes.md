## Approach

A palindrome reads the same forwards and backwards, so it is symmetric around a center.
Rather than checking all `O(n^2)` substrings individually, the algorithm expands outward
from every possible center and counts palindromes as they are found.

For each position `i`, there are two center cases:

- **Odd-length palindromes**: center is position `i` itself; count the single character
  and expand left and right while characters match
- **Even-length palindromes**: center is between positions `i` and `i+1`; expand only if
  `s[i] == s[i+1]`, then count the pair and keep expanding

The expansion is done by a helper that walks outward until a mismatch or boundary,
counting each valid palindrome found along the way. Since expansion can go at most
`O(n)` steps from any center, total work is `O(n) × O(n) = O(n^2)`.

## Complexity

- Time: `O(n²)`
- Space: `O(1)`
