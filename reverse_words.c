// Problem 151
// Reverse Words in a String
// https://leetcode.com/problems/reverse-words-in-a-string/description/

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