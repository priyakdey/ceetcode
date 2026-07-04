/**
 * @number: 21
 * @title: Merge Two Sorted Lists
 * @difficulty: Easy
 * @tags: linked-list, recursion
 * @link: https://leetcode.com/problems/merge-two-sorted-lists/description
 */

#include <stdlib.h>

/// Definition of a singly linked list
struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode *mergeTwoLists(struct ListNode *list1, struct ListNode *list2) {
    if (list1 == NULL && list2 == NULL)
        return NULL;
    if (list1 == NULL)
        return list2;
    if (list2 == NULL)
        return list1;

    struct ListNode *head = list1;
    struct ListNode *prev = list1;
    struct ListNode *curr1 = list1->next;
    struct ListNode *curr2 = list2;

    if (list2->val < list1->val) {
        head = list2;
        prev = list2;
        curr1 = list2->next;
        curr2 = list1;
    }

    while (curr1 != NULL && curr2 != NULL) {
        if (curr1->val < curr2->val) {
            prev = curr1;
            curr1 = curr1->next;
        } else {
            struct ListNode *next = curr2->next;
            prev->next = curr2;
            curr2->next = curr1;
            prev = curr2;
            curr2 = next;
        }
    }

    if (curr2 != NULL) {
        prev->next = curr2;
    }

    return head;
}
