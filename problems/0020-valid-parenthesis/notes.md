## Approach

Classic **stack** problem. Scan the string left to right:
- On an opening bracket (`(`, `{`, `[`), push it onto the stack.
- On a closing bracket, the top of the stack must be its matching
  opener - pop and compare. An empty stack (a closer with nothing to
  close) or a mismatched opener means the string is invalid.

A closer can only be valid against the *most recently seen* unmatched
opener - that LIFO ordering is exactly what a stack gives, which is why
it's the natural fit. Nesting like `([{}])` resolves from the inside
out as each pair pops.

At the end the stack must be **empty** (`cursor == 0`). A leftover
opener (e.g. `(((`) was never closed, so the string is invalid.

The stack is a VLA sized to `strlen(s)` - the whole string could be
openers, so that's the tight upper bound and avoids any resizing.

## Complexity

- Time: `O(n)`
- Space: `O(n)`
