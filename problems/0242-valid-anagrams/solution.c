/**
 * @number: 242
 * @title: Valid Anagram
 * @difficulty: Easy
 * @tags: hash-table, string, sorting
 * @link: https://leetcode.com/problems/valid-anagram
 */

#include <stdbool.h>

// NOTE: s and t consist of lowercase English letters
bool isAnagram(char *s, char *t) {
    int char_freq[26] = {0};

    while (*s != '\0') {
        char ch = *s;
        int index = (int)ch - (int)'a';
        char_freq[index]++;
        s++;
    }

    while (*t != '\0') {
        char ch = *t;
        int index = (int)ch - (int)'a';
        char_freq[index]--;
        t++;
    }

    for (int i = 0; i < 26; i++) {
        if (char_freq[i] != 0)
            return false;
    }

    return true;
}
