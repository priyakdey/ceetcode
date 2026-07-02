## Approach

Rotate 90° clockwise **in place** in two passes, no extra matrix.

First **transpose** across the main diagonal - swap `matrix[row][col]`
with `matrix[col][row]`. The inner loop starts at `col = row` so each
pair is touched once; starting at `0` would swap every pair twice and
undo the work.

Transposing turns rows into columns but leaves the result mirrored, so
the second pass **reverses each row** - swap column `col` with column
`cols - 1 - col`, walking only to the midpoint (`cols / 2`).

Transpose then horizontal-flip composes exactly into a clockwise
quarter turn: the top row ends up on the right, as required.

## Complexity

- Time: `O(n^2)` 
- Space: `O(1)`
