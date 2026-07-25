## Approach

**Two-pointer technique:**

- `curr`: pointer iterating through the array

- `insert_at`: pointer tracking where to place the next unique element  

- `last_uniq`: tracks the last unique value written

Since the array is sorted, duplicates are consecutive. Scan through once, 
placing each value that differs from `last_uniq` at position `insert_at`. 
Return the final count of unique elements.

## Complexity

- Time: `O(n)`
- Space: `O(1)`
