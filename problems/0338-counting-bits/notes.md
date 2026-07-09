## Approach

For each `i` in `0..n` we want its popcount, built from earlier answers. Stripping the
highest set bit off `i` leaves a smaller number already computed, so
`popcount(i) = 1 + popcount(i - prev_power)`, where `prev_power` is the largest power of
two `<= i`. We sweep upward tracking `prev_power`/`next_power`: when `i` hits `next_power`
it's a fresh power of two (popcount `1`), so we record `1` and roll the powers forward;
otherwise it's one lookup plus one. Single pass, `O(1)` work per element.

## Complexity

- Time: `O(n)`
- Space: `O(1)` extra (output array aside)
