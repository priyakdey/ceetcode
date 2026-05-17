## Approach

Slide a window `[start, curr]` across the string, expanding `curr`
rightward one character at a time. The invariant: every character
inside the window is unique.

To enforce the invariant in O(1) per step, keep a lookup table
mapping each character to the index where it was last seen. ASCII
gives us a fixed 256-slot array - no hashing, just direct indexing.
Initialize to `-1` to mean "not seen yet."

For each new character `s[curr]`:

- If we've seen it before *and* its last position is inside the
  current window (`table[c] >= start`), it's a duplicate. Shrink the
  window from the left by jumping `start` to one past the previous
  occurrence. This collapses what would be a per-character left-edge
  scan into a single index update.
- Otherwise the window is still valid - just extend it.

Either way, record the current position in `table[c]` and update
`maxLength`.

The `table[c] >= start` check is the subtle bit: a character might be
in the table from a much earlier position that's already been
abandoned by the window. Without the guard, we'd incorrectly shrink
the window backwards.

The final `MAX` after the loop catches the case where the longest
window ends at the last character - no duplicate ever triggered an
update for it.

## Complexity

- Time: `O(n)` - each character is visited exactly once; `start` only
  moves forward, never backward.
- Space: `O(1)` - the lookup table is a fixed 256 ints regardless of
  input size. Technically `O(k)` where `k` is the alphabet, but `k` is
  a constant here.
