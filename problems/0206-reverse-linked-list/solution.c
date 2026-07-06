/**
 * @number: 206
 * @title: Reverse Linked List
 * @difficulty: Easy
 * @tags: linked-list, recursion
 * @link: https://leetcode.com/problems/reverse-linked-list
 */

#include <stdlib.h>

typedef struct ListNode ListNode;

struct ListNode {
    int val;
    ListNode *next;
};

ListNode *reverseList(ListNode *head) {
    ListNode *prev = NULL;
    ListNode *curr = head;

    while (curr != NULL) {
        ListNode *next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }

    return prev;
}

/*
This is a recursive solution

ListNode *reverseListRecursiveInternal(ListNode *curr, ListNode *prev) {
    if (curr == NULL)
        return curr;

    ListNode *next = curr->next;
    curr->next = prev;
    return reverseListRecursiveInternal(next, curr);
}

ListNode *reverseListRecursive(ListNode *head) {
    return reverseListRecursiveInternal(head, NULL);
}

*/
