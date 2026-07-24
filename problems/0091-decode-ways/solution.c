/**
 * @number: 91
 * @title: Decode Ways
 * @difficulty: Medium
 * @tags: string, dynamic programming
 * @link: https://leetcode.com/problems/decode-ways
 */

#include <string.h>

int numDecodings_internal(char *s, int n, int index, int *cache) {
    if (index == n)
        return 1;

    if (s[index] == '0')
        return 0;

    if (cache[index] != -1)
        return cache[index];

    // Considering string of length 1
    int count = numDecodings_internal(s, n, index + 1, cache);

    // Considering string of length 2 which starts with 1
    if (s[index] == '1' && index < n - 1) {
        count += numDecodings_internal(s, n, index + 2, cache);
    }

    // Considering string of length 2 which starts with 2
    if (s[index] == '2' && index < n - 1 && s[index + 1] <= '6') {
        count += numDecodings_internal(s, n, index + 2, cache);
    }

    cache[index] = count;
    return count;
}

int numDecodings(char *s) {
    int n = strlen(s);
    int cache[101] = {0};

    for (int i = 0; i < 101; i++) {
        cache[i] = -1;
    }

    return numDecodings_internal(s, n, 0, cache);
}
