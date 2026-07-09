/**
 * @number: 268
 * @title: Missing Number
 * @difficulty: Easy
 * @tags: array, hashtable, maths, binary-seach, bit-manipulation, sorting
 * @link: https://leetcode.com/problems/missing-number
 */

int missingNumber(int *nums, int numsSize) {
    int missing = numsSize * (numsSize + 1) / 2;

    for (int i = 0; i < numsSize; i++) {
        missing -= nums[i];
    }

    return missing;
}
