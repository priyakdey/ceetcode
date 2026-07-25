/**
 * @number: 151
 * @title: Reverse Words in a String
 * @difficulty: Medium
 * @tags: two pointers, string
 * @link: https://leetcode.com/problems/reverse-words-in-a-string
 */

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Returns a substring of s from start to end indices - both inclusive
char *substring(char *s, int start, int end) {
    int length = end - start + 1 + 1;
    char *substring = (char *)malloc(sizeof(char) * length);

    for (int i = start; i <= end; i++) {
        substring[i - start] = s[i];
    }

    substring[length - 1] = '\0';

    return substring;
}

typedef struct {
    char **data;
    uint32_t size;
    uint32_t capacity;
} Array;

Array *array_init(uint32_t capacity) {
    Array *array = (Array *)calloc(1, sizeof(Array));
    char **data = (char **)calloc(capacity, sizeof(char *));

    array->capacity = capacity;
    array->data = data;

    return array;
}

void array_add(Array *array, char *s) {
    if (array->size == array->capacity) {
        array->capacity = array->capacity << 1;
        array->data =
            (char **)realloc(array->data, array->capacity * sizeof(char *));
    }

    array->data[array->size++] = s;
}

char *reverseWords(char *s) {
    int length = strlen(s);

    // This leads to too many reallocs in case of high word count
    Array *array = array_init(10);

    int i = 0;
    int resultSize = 0;

    while (i < length) {
        // skip over all the spaces
        while (i < length && isspace(s[i])) {
            i++;
        }

        if (i == length)
            break;

        int start = i;

        while (i < length && !isspace(s[i])) {
            i++;
        }

        char *word = substring(s, start, i - 1);
        printf("word = \"%s\"\n", word);
        array_add(array, word);

        // word length without the null terminator
        resultSize += i - start;
    }

    // adding space for (words count - 1) spaces and 1 null terminator
    resultSize = resultSize + array->size - 1 + 1;

    char *result = (char *)malloc(sizeof(char) * resultSize);
    int curr = 0;

    for (int i = array->size - 1; i >= 0; i--) {
        char *word = array->data[i];
        int n = strlen(word);
        for (int j = 0; j < n; j++) {
            result[curr++] = word[j];
        }

        if (i > 0)
            result[curr++] = ' ';
    }

    result[resultSize - 1] = '\0';

    return result;
}
