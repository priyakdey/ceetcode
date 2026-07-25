## Approach

**Buffering with stateful cursor:**

Maintain an internal buffer (`buf4`) to cache up to 4 characters read from `read4`. Track cursor position and valid size in the buffer across multiple calls. For each character requested:

1. If buffer is empty or cursor exhausted, refill via `read4`

2. If no more data available, return

3. Copy character from buffer to result and advance cursor

4. Return total characters read

Handles multiple calls by preserving unread characters in the buffer for subsequent calls.

## Complexity

- Time: `O(n)` -  where `n` is the number of characters to read - each character copied once.
- Space: `O(1)` - fixed internal buffer of 4 characters.
