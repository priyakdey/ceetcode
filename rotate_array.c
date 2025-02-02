// Problem 189
// Rotate Array
// https://leetcode.com/problems/rotate-array/description/

// Reverse an array in-place from start to end, exclusive.
void reverse(int *nums, int start, int end)
{
    while (start < end - 1)
    {
        int temp = nums[start];
        nums[start] = nums[end - 1];
        nums[end - 1] = temp;
        start++;
        end--;
    }
}

void rotate(int *nums, int numsSize, int k)
{
    k = k % numsSize;
    if (k == 0)
    {
        return;
    }

    reverse(nums, 0, numsSize);
    reverse(nums, 0, k);
    reverse(nums, k, numsSize);
}