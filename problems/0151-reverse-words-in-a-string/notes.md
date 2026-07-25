## Approach

**Two-pass algorithm with dynamic array:**

1. Parse the string to extract words, skipping leading/trailing and intermediate spaces

2. Store each word in a dynamic array

3. Iterate the array in reverse order, concatenating words with single spaces between them

## Complexity

- Time: `O(n)`
- Space: `O(n)`
