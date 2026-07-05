/**
 * @number: 70
 * @title: Climbing Stairs
 * @difficulty: Easy
 * @tags: maths, dynamic-programming, memoization
 * @link: https://leetcode.com/problems/climbing-stairs/description
 */

int climbStairs(int n) {
    long a = 1, b = 2;

    while (n > 1) {
        long temp = (long)a + b;
        a = b;
        b = temp;
        n--;
    }

    return a;
}
