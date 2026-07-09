## Approach

Two strings are anagrams when they contain exactly the same characters with the same
frequencies — order does not matter. Since the input is restricted to lowercase English
letters, a fixed array of 26 counters is enough to track those frequencies without a
hash table.

The counts are folded into a single array in one pass each:

- walk `s`, incrementing the counter for each character (`ch - 'a'` maps `a..z` to
  `0..25`)
- walk `t`, decrementing the same counters

If the two strings are anagrams every counter cancels back to `0`; any surviving
non-zero entry means some letter appears a different number of times (this also catches
differing lengths, since an extra character leaves an imbalance). A final scan over the
26 counters confirms they are all zero.

## Complexity

- Time: `O(n)`
- Space: `O(1)`
