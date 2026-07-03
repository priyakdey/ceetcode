/**
 * @number: 53
 * @title: Maximum subarray
 * @difficulty: Medium
 * @tags: array, divide-and-conquer, dynamic-progamming
 * @link: https://leetcode.com/problems/maximum-subarray/description/
 */

#include <assert.h>

#include <sys/param.h>

int maxSubArray(int *nums, int numsSize) {
    assert(numsSize > 0);

    int maxSum = nums[0];
    int currSum = nums[0];

    for (int i = 1; i < numsSize; i++) {
        currSum = MAX(currSum + nums[i], nums[i]);
        maxSum = MAX(maxSum, currSum);
    }

    return maxSum;
}
