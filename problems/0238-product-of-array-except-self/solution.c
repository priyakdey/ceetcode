/**
 * @number: 238
 * @title: Product of Array Except Self
 * @difficulty: Hard
 * @tags: array, prefix sum
 * @link: https://leetcode.com/problems/product-of-array-except-self
 */

#include <stdlib.h>

int *productExceptSelf(int *nums, int numsSize, int *returnSize) {
    int *result = (int *)malloc(sizeof(int) * numsSize);
    *returnSize = numsSize;

    for (int i = 0; i < numsSize; i++) {
        result[i] = 1;
    }

    int prefix = 1, suffix = 1;
    int left = 1, right = numsSize - 2;

    while (left < numsSize) {
        prefix = prefix * nums[left - 1];
        result[left] = result[left] * prefix;
        left++;

        suffix = suffix * nums[right + 1];
        result[right] = result[right] * suffix;
        right--;
    }

    return result;
}
