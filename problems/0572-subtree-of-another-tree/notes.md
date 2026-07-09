## Approach

`subRoot` is a subtree of `root` if, at some node of `root`, the tree rooted there is
identical to `subRoot`. So combine two recursions: `isSameTree` checks structural and
value equality of two trees node-for-node, and `isSubtree` tries that match at the current
node, then recurses into the left and right children. A `NULL` root can't contain a
non-empty subtree, so it returns `false` and stops the descent.

**NOTE**: The `string-matching`/`hash-function` tags hint at the `O(m + n)` 
trick: serialize both trees and run a substring/Merkle-hash match. 
This solution takes the plain DFS route.

## Complexity

- Time: `O(m * n)`
- Space: `O(m + n)`

