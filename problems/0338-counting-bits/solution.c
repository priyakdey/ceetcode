/**
 * @number: 338
 * @title: Counting Bits
 * @difficulty: Easy
 * @tags: dynamic-programming, bit-manipulation
 * @link: https://leetcode.com/problems/counting-bits
 */

#include <stdlib.h>

int *countBits(int n, int *returnSize) {
    int *result = (int *)malloc((n + 1) * sizeof(int));
    *returnSize = n + 1;

    result[0] = 0;

    // Powers of 2. Even though 0 is not not, it just
    // avoids a bunch of if-else
    int prev_power = 0;
    int next_power = 1;

    for (int i = 1; i <= n; i++) {
        if (i == next_power) {
            result[i] = 1;
            prev_power = next_power;
            next_power = next_power << 1;
        } else {
            result[i] = 1 + result[i - prev_power];
        }
    }

    return result;
}
