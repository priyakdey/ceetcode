/**
 * @number: 152
 * @title: Maximum Product Subarray
 * @difficulty: Medium
 * @tags: array, dynamic programming
 * @link: https://leetcode.com/problems/maximum-product-subarray
 */

#include <stdarg.h>

#include <sys/param.h>

int find_max(int count, ...) {
    va_list args;

    va_start(args, count);

    int max_value = va_arg(args, int);

    for (int i = 1; i < count; i++) {
        int current = va_arg(args, int);
        max_value = MAX(max_value, current);
    }

    va_end(args);

    return max_value;
}

int find_min(int count, ...) {
    va_list args;

    va_start(args, count);

    int min_value = va_arg(args, int);

    for (int i = 1; i < count; i++) {
        int current = va_arg(args, int);
        min_value = MIN(min_value, current);
    }

    va_end(args);

    return min_value;
}

int maxProduct(int *nums, int numsSize) {
    int curr_min = nums[0];
    int curr_max = nums[0];
    int overall_max = nums[0];

    for (int i = 1; i < numsSize; i++) {
        int min;
        int max;

        if (nums[i] != 0) {
            min = find_min(3, curr_min * nums[i], curr_max * nums[i], nums[i]);
            max = find_max(3, curr_min * nums[i], curr_max * nums[i], nums[i]);
            overall_max = find_max(2, overall_max, max);
        } else {
            min = 1;
            max = 1;
            overall_max = find_max(2, overall_max, 0);
        }

        curr_min = min;
        curr_max = max;
    }

    return overall_max;
}
