// https://leetcode.com/problems/special-array-i/decription/

#include <stdbool.h>
#include <stdio.h>

bool isArraySpecial(int *nums, int numsSize)
{
    for (int i = 1; i < numsSize; i++)
    {
        if (((nums[i] & 1) == 0 && (nums[i - 1] & 1) == 0) || ((nums[i] & 1) != 0 && (nums[i - 1] & 1) != 0))
        {
            return false;
        }
    }

    return true;
}

int main()
{
    int nums[2] = {2, 10};
    bool a = isArraySpecial(nums, 2);
    printf("%d\n", a);
    return 0;
}