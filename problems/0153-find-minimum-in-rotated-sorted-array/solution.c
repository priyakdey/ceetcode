/**
 * @number: 153
 * @title: Find Minimum in Rotated Sorted Array
 * @difficulty: Medium
 * @tags: array, binary search
 * @link: https://leetcode.com/problems/find-minimum-in-rotated-sorted-array
 */

#include <sys/param.h>

int findMin(int *nums, int numsSize) {
    int min = nums[0];

    int left = 0, right = numsSize - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] <= nums[numsSize - 1]) {
            min = MIN(min, nums[mid]);
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    return min;
}
