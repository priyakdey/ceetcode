/**
 * @number: 443
 * @title: String Compression
 * @difficulty: Medium
 * @tags: two pointers, string
 * @link: https://leetcode.com/problems/string-compression
 */

int compress(char *chars, int charsSize) {
    int curr = 0;
    int insert_at = 0;

    while (curr < charsSize) {
        int start = curr;
        char ch = chars[start];

        while (curr < charsSize && chars[curr] == ch) {
            curr++;
        }

        int streak = curr - start;

        chars[insert_at++] = ch;

        if (streak == 1)
            continue;

        start = insert_at;

        while (streak > 0) {
            chars[insert_at++] = (char)((streak % 10) + (int)'0');
            streak = streak / 10;
        }

        int end = insert_at - 1;

        while (start < end) {
            int temp = chars[start];
            chars[start] = chars[end];
            chars[end] = temp;
            start++;
            end--;
        }
    }

    return insert_at;
}
