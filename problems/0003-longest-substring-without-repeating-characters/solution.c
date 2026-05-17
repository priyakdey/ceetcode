/**
 * @number: 3 
 * @title: Longest Substring Without Repeating Characters 
 * @difficulty: Medium
 * @tags: hashtable, string, sliding-window 
 * @link: https://leetcode.com/problems/longest-substring-without-repeating-characters/description/ 
 */

#include <string.h>
#include <sys/param.h>

int lengthOfLongestSubstring(char* s) 
{
    int table[256] = {0};        

    for (int i = 0; i < 256; i++) 
    {
       table[i] = -1; 
    }

    int start = 0, maxLength = 0;
    int curr = 0;
    size_t length = strlen(s);

    while (curr < length) 
    {
        int c = s[curr];

        if (table[c] != -1 && table[c] >= start) 
        {
            maxLength = MAX(maxLength, curr - 1 - start + 1);
            start = table[c] + 1;
        }
        table[c] = curr;
        curr++;
    }

    maxLength = MAX(maxLength, curr - 1 - start + 1);

    return maxLength;
}

