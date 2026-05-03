/**
 * @number: 151
 * @title: Reverse Words in a String
 * @difficulty: TODO
 * @tags: TODO
 * @draft: true
 * @link: https://leetcode.com/problems/reverse-words-in-a-string/description/
 */

void reverse(char *s, int start, int end)
{
    while (start < end - 1)
    {
        char temp = s[start];
        s[start] = s[end - 1];
        s[end - 1] = temp;
        start++;
        end--;
    }
}

char *reverseWords(char *s)
{
}