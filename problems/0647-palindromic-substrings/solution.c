/**
 * @number: 647
 * @title: Palindromic Substrings
 * @difficulty: Medium
 * @tags: two-pointer, strings, dynamic-programming
 * @link: https://leetcode.com/problems/palindromic-substrings
 */

#include <string.h>

int countPalindromicSubString(char *s, unsigned long length, int left,
                              int right) {
    int count = 0;

    while (left >= 0 && right < length && s[left] == s[right]) {
        left--;
        right++;
        count++;
    }

    return count;
}

int countSubstrings(char *s) {
    unsigned long length = strlen(s);

    int count = 0;

    for (int i = 0; i < length; i++) {
        int left = i - 1, right = i + 1;
        count++;
        count += countPalindromicSubString(s, length, i - 1, i + 1);

        if (i < length - 1 && s[i] == s[i + 1]) {
            count++;
            count += countPalindromicSubString(s, length, i - 1, i + 2);
        }
    }

    return count;
}
