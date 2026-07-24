## Approach

DFS post-order traversal where each node tracks two values:
- The global maximum path sum seen so far (can bend at any node)
- The maximum path sum starting at this node extending downward in one direction (for parent to extend)

At each node:
1. Recurse into both children (if they exist), initialize left/right sums to 0 if NULL
2. Update global max with all four possibilities: just node, node+left, node+right, node+left+right (the bend case)
3. Return to parent the best single-direction extension: max(node, node+left, node+right)

By initializing child sums to 0 only when NULL (rather than returning 0 from 
NULL nodes), we avoid extending paths through non-existent subtrees. When 
combining sums, the max_int comparisons ensure we never force negative 
contributions - if a child path is negative, it won't be chosen as the 
best single direction.

Handle leaf nodes explicitly to simplify recursion base case.

## Complexity

- Time: `O(n)`
- Space: `O(h)`
