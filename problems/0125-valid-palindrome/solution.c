/**
 * @number: 125
 * @title: Valid Palindrome
 * @difficulty: Easy
 * @tags: two-pointers, string
 * @link: https://leetcode.com/problems/valid-palindrome
 */

#include <ctype.h>
#include <stdbool.h>
#include <string.h>

bool isPalindrome(char *s) {
    int length = strlen(s);

    int left = 0, right = length - 1;

    while (left < right) {
        char l = s[left], r = s[right];

        if (!isalnum(l)) {
            left++;
            continue;
        }

        if (!isalnum(r)) {
            right--;
            continue;
        }

        if (l != r && (l ^ 32) != r)
            return false;

        left++;
        right--;
    }

    return true;
}
