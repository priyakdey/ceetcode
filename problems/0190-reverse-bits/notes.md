## Approach

Build the result one bit at a time. For each of the 32 positions, pull
`n`'s lowest bit (`n & 1`) and place it at the mirrored position
(`31 - i`) in `reverse` with a shift, then drop that bit off `n`
(`n >> 1`).

After 32 iterations every bit has been moved to its reflected slot, so
`reverse` holds `n` bit-reversed.

## Complexity

- Time: `O(1)` - always 32 iterations
- Space: `O(1)`
