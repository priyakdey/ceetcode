/**
 * @number: 191
 * @title: Number of 1s bit
 * @difficulty: Easy
 * @tags: divide-and-conquer, bit-manipulation
 * @link: https://leetcode.com/problems/number-of-1-bits
 */

int hammingWeight(int n) {
    int set = 0;

    for (int i = 0; i < 32; i++) {
        set += n & 1;
        n = n >> 1;
    }

    return set;
}
