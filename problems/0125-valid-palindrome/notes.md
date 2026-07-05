## Approach

A palindrome reads the same both ways, so we walk two pointers inward from each end and
compare, ignoring everything that isn't alphanumeric:

- `left` character not alphanumeric → skip it, `left++`
- `right` character not alphanumeric → skip it, `right--`
- both alphanumeric → they must match; advance both inward

The case-insensitive check is `l != r && (l ^ 32) != r`. ASCII upper- and lowercase
letters differ only in bit `5` (value `32`), so `l ^ 32` flips a letter's case; a pair
matches if it is equal outright or equal after that flip. Digits only reach the XOR test
when they already differ, and flipping a letter's case never produces a digit, so there
are no false matches.

Any mismatch returns `false` immediately; if the pointers cross without one, the string
is a palindrome.

## Complexity

- Time: `O(n)`
- Space: `O(1)`
