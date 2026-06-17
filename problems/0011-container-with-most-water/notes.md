## Approach

This is a classic example of two-pointer problem. We have two pointers on extreme ends of the
array(`left` and `right`). The area formed is: `min(height[left], height[right]) * width`,
where `width = right - left`.

The width can only shrink when we converge the two pointers, so the only way to maximize the area
is to increase the bounding(shorter) height. Moving the taller pointer inwards keeps the same
min height but reduces the width - guaranteed worse. So we always move the pointer at which
the height is less.


## Complexity

- Time: `O(n)` - a single pass over the array 
- Space: `O(1)` - no extra allocations needed 

