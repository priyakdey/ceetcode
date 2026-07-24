## Approach

**In-order traversal** of a BST yields elements in sorted order. Use an explicit **stack** to traverse iteratively: push all left children to reach the leftmost node, then pop to visit the next smallest element, decrement `k`, and move to the right subtree.

The pattern repeats: from each popped node's right child, descend left and push onto the stack. When `k` reaches zero, the current node is the answer. A fixed-capacity stack sized to `k` suffices since we only need to store nodes on the path from root to current position (at most `height` nodes, typically `O(log n)`).

## Complexity

- Time: `O(n)`
- Space: `O(h)`
