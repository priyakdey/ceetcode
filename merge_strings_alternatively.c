// https://leetcode.com/problems/merge-strings-alternately/description

#include <string.h>

char *mergeAlternately(char *word1, char *word2)
{
    size_t len1 = strlen(word1);
    size_t len2 = strlen(word2);

    int len = len1 + len2 + 1;
    char *result = (char *)malloc(len);

    size_t i = 0, j = 0, k = 0;

    while (i < len1 && j < len2)
    {
        result[k++] = word1[i++];
        result[k++] = word2[j++];
    }

    while (i < len1)
    {
        result[k++] = word1[i++];
    }

    while (j < len2)
    {
        result[k++] = word2[j++];
    }

    result[k] = '\0';
    return result;
}