// Problem 80
// https://leetcode.com/problems/remove-duplicates-from-sorted-array-ii/description

#include <assert.h>
#include <stdlib.h>

#include <sys/param.h>

int removeDuplicates(int *nums, int numsSize)
{
    assert(nums != NULL);
    assert(numsSize > 0);

    int last_uniq = nums[0] - 1;
    int insert_at = 0, curr = 0;

    while (curr < numsSize)
    {
        int start = curr;
        while (curr < numsSize && nums[curr] == nums[start])
        {
            curr++;
        }

        int count = MIN(curr - start, 2);
        for (int i = 0; i < count; i++)
        {
            nums[insert_at++] = nums[start];
        }
    }

    return insert_at;
}