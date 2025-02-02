// 443. String Compression
// https://leetcode.com/problems/string-compression/description

int compress(char *chars, int charsSize)
{
    int cursor = 0;
    int insert_at = 0;

    while (cursor < charsSize)
    {
        int start = cursor;
        while (cursor < charsSize && chars[cursor] == chars[start])
        {
            cursor++;
        }

        int count = cursor - start;
        chars[insert_at++] = chars[start];
        start = insert_at;   // digit count starts from here
        int end = insert_at; // digit count ends here
        while (count > 0)
        {
            chars[insert_at++] = count % 10 + '0';
            count /= 10;
            end = insert_at;
        }

        // reverse the digits in buffer from end to start
        while (start < end)
        {
            char temp = chars[start];
            chars[start] = chars[end];
            chars[end] = temp;
            start++;
            end--;
        }
    }

    return insert_at;
}