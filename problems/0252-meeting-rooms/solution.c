/**
 * @number: 252
 * @title: Meeting Rooms
 * @difficulty: Easy
 * @tags: array, sorting
 * @link: https://leetcode.com/problems/meeting-rooms
 */

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

int compare_fn(const void *a, const void *b) {
    int **first = (int **)a;
    int **second = (int **)b;

    int first_start = (*first)[0];
    int first_end = (*first)[1];

    int second_start = (*second)[0];
    int second_end = (*second)[1];

    if (first_start < second_start) {
        return -1;
    } else if (first_start == second_start) {
        return first_end - second_end;
    } else {
        return 1;
    }

    return -1; // UNREACHABLE
}

bool canAttendMeetings(int **intervals, int intervalsSize,
                       int *intervalsColSize) {
    if (intervalsSize < 2)
        return true;

    qsort(intervals, intervalsSize, sizeof(intervals[0]), compare_fn);

    int prev_end = intervals[0][1];
    for (int i = 1; i < intervalsSize; i++) {
        assert(intervalsColSize[i] == 2);

        if (intervals[i][0] < prev_end) {
            return false;
        }

        prev_end = intervals[i][1];
    }

    return true;
}
