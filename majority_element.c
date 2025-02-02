// 169. Majority Element
// https://leetcode.com/problems/majority-element/description/

#include <assert.h>
#include <stdlib.h>

int majorityElement(int *nums, int numsSize)
{
    assert(nums != NULL);
    assert(numsSize > 0);

    int bias = 1;
    int majorityElement = nums[0];

    for (int i = 1; i < numsSize; i++)
    {
        int incr_by = majorityElement == nums[i] ? 1 : -1;
        bias += incr_by;

        if (bias == 0)
        {
            majorityElement = nums[i];
            bias = 1;
        }
    }

    int count = 0;
    for (int i = 0; i < numsSize; i++)
    {
        if (nums[i] == majorityElement)
        {
            count++;
        }
    }

    assert(count > numsSize / 2);
    return majorityElement;
}