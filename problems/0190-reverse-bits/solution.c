/**
 * @number: 190
 * @title: Reverse Bits
 * @difficulty: Easy
 * @tags: divide-and-conquer, bit-manipulation
 * @link: https://leetcode.com/problems/reverse-bits
 */

int reverseBits(int n) {
    int reverse = 0;

    for (int i = 0; i < 32; i++) {
        reverse = reverse | ((n & 1) << (32 - i - 1));
        n = n >> 1;
    }

    return reverse;
}
