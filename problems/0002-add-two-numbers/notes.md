## Approach

Walk both lists in lockstep, summing corresponding digits along with a
running `carry`. Since digits are stored least-significant-first, the
addition flows naturally left-to-right - exactly how you'd do it by
hand, just without flipping the number around.

The loop condition `curr1 || curr2 || carry` handles all three "tail"
cases uniformly:
- one list runs out before the other - treat the missing digit as 0
- both run out but a carry remains (e.g. `999 + 1 = 1000`) - emit one
  more node for the carry

This collapses what would otherwise be three separate loops (both
running, l1 longer, l2 longer) into a single loop body. Missing
operands are read as 0, which is mathematically what you want anyway.

A **dummy head** node sidesteps the "is this the first result node?"
branch - we always have a valid `tail` to append to. At the end, the
real list starts at `dummy.next`.

The result is built as a fresh list rather than mutating either input.
This is more memory than strictly necessary (you could reuse l1's
nodes in place), but it keeps the function pure with respect to its
inputs - cleaner contract, easier to reason about, no surprises for
the caller.

## Complexity

- Time: `O(max(m, n))` - single pass, where `m` and `n` are the
  lengths of the two lists. One extra iteration at most for a trailing
  carry.
- Space: `O(max(m, n))` - one new node per output digit. Could be done
  in-place by mutating `l1`, but the stub does return a `ListNode` and 
  doesn't mark inputs as `const` - fine for LeetCode, but a real-world 
  API should declare that contract clearly.

