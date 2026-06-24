/**
 * @number: 5
 * @title: Longest Palindromic Substring
 * @difficulty: Medium
 * @tags: two-pointers, string, dynamic-programming
 * @link: https://leetcode.com/problems/longest-palindromic-substring/description/
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>


char* longestPalindrome (char* s) {
    size_t length = strlen (s);

    int left = 0, right = 1;
    int substringLength = 1;

    for (int i = 0; i < length; i++) {
        // odd length
        int l = i - 1, r = i + 1;
        while (l >= 0 && r < length && s[l] == s[r]) {
            l--;
            r++;
        }

        if (r - (l + 1) > substringLength) {
            substringLength = r - (l + 1);
            left            = l + 1;
            right           = r;
        }

        // even length
        if (i < length - 1 && s[i] == s[i + 1]) {

            l = i - 1;
            r = i + 2;

            while (l >= 0 && r < length && s[l] == s[r]) {
                l--;
                r++;
            }

            if (r - (l + 1) > substringLength) {
                substringLength = r - (l + 1);
                left            = l + 1;
                right           = r;
            }
        }
    }

    char* substring = (char*)malloc ((substringLength + 1) * sizeof (char));
    memcpy (substring, s + left, substringLength);
    substring[substringLength] = '\0';

    return substring;
}
