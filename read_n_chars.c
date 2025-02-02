// https://leetcode.com/problems/read-n-characters-given-read4-ii-call-multiple-times/description/

#include <stdlib.h>

/**
 * The read4 API is defined in the parent class Reader4.
 *     int read4(char *buf4);
 */
int read4(char *buf4)
{
    return -1;
}

#define BUF4_CAPACITY 4

typedef struct
{
    char *buf4;
    size_t cursor;
    size_t size;
} Solution;

/** initialize your data structure here. */
Solution *solutionCreate()
{
    Solution *obj = (Solution *)malloc(sizeof(Solution));
    obj->buf4 = (char *)malloc(sizeof(char) * BUF4_CAPACITY);
    obj->cursor = 0;
    obj->size = 0;
    return obj;
}

/**
 * @param buf Destination buffer
 * @param n   Number of characters to read
 * @return    The number of actual characters read
 */
int _read(Solution *obj, char *buf, int n)
{
    int chars_read = 0;
    while (n > 0)
    {
        if (obj->size == 0 || obj->cursor == obj->size)
        {
            obj->size = read4(obj->buf4);
            obj->cursor = 0;
        }

        if (obj->size == 0)
        {
            break;
        }

        *buf++ = obj->buf4[obj->cursor++];
        n--;
        chars_read++;
    }

    return chars_read;
}