/**
 * @number: 0033
 * @title: Search in Rotated Sorted Array
 * @difficulty: Medium
 * @tags: array, binary-search
 * @link: https://leetcode.com/problems/search-in-rotated-sorted-array
 */

int search(int *nums, int numsSize, int target) {
    int left = 0, right = numsSize - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (target == nums[mid]) {
            return mid;
        }

        if (nums[mid] > nums[right]) {
            if (target < nums[mid]) {
                if (target <= nums[right]) {
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            } else {
                left = mid + 1;
            }
        } else {
            if (target < nums[mid]) {
                right = mid - 1;
            } else {
                if (target <= nums[right]) {
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }
        }
    }

    return -1;
}
