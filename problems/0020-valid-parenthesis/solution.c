/**
 * @number: 20
 * @title: Valid Parenthesis
 * @difficulty: Easy
 * @tags: string, stack
 * @link: https://leetcode.com/problems/valid-parentheses/description
 */

#include <stdbool.h>
#include <string.h>


bool isValid (char* s) {
    int capacity = (int)strlen (s);

    char stack[capacity];
    int cursor = 0;

    while (*s != '\0') {
        char c = *s;

        if (c == '(' || c == '{' || c == '[') {
            stack[cursor++] = c;
        } else {
            if (cursor == 0)
                return false;

            char p = stack[--cursor];

            if (c == ')' && p != '(')
                return false;
            if (c == '}' && p != '{')
                return false;
            if (c == ']' && p != '[')
                return false;
        }

        s++;
    }

    return cursor == 0;
}
