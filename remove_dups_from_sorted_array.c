// Problem 26
// https://leetcode.com/problems/remove-duplicates-from-sorted-array/description/

#include <assert.h>
#include <stdlib.h>

int removeDuplicates(int *nums, int numsSize)
{
    assert(nums != NULL);
    assert(numsSize > 0);

    int last_uniq = nums[0] - 1;
    int insert_at = 0, curr = 0;

    while (curr < numsSize)
    {
        if (nums[curr] != last_uniq)
        {
            nums[insert_at++] = nums[curr];
            last_uniq = nums[curr];
        }

        curr++;
    }

    return insert_at;
}