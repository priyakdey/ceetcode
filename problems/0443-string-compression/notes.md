## Approach

Use two pointers to compress in-place. Iterate through the string with `curr` 
to group consecutive identical characters, counting the streak length. 
Write the character at `insert_at`, and if the streak is greater than 1, 
extract the count digits using modulo 10 (producing them in reverse order), 
then reverse the digit sequence back to the correct order. Advance `insert_at` 
accordingly and return the final compressed length.

## Complexity

- Time: `O(n)`
- Space: `O(1)`
