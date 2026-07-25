## Approach

**Grouping with bounded duplication:**

- `start`: marks the beginning of a group of identical elements

- `curr`: advances through the array to find the end of the current group

- `insert_at`: tracks where to place the next batch of elements

For each group of identical elements:

1. Count how many there are (`curr - start`)

2. Allow at most 2 copies (using `MIN(curr - start, 2)`)

3. Write that many copies to position `insert_at`

4. Move to the next group

## Complexity

- Time: `O(n)`
- Space: `O(1)`
