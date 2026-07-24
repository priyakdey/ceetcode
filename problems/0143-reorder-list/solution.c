/**
 * @number: 143
 * @title: Reorder List
 * @difficulty: Medium
 * @tags: linked-list, two-pointers, stack, recursion
 * @link: https://leetcode.com/problems/reorder-list
 */

#include <stdlib.h>

typedef struct ListNode ListNode;

struct ListNode {
    int val;
    ListNode *next;
};

ListNode *find_mid(ListNode *head) {
    ListNode *curr = head;
    ListNode *mid = head;

    while (curr != NULL && curr->next != NULL) {
        mid = mid->next;
        curr = curr->next->next;
    }

    return mid;
}

ListNode *reverse_list(ListNode *head) {
    ListNode *curr = head;
    ListNode *prev = NULL;

    while (curr != NULL) {
        ListNode *next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }

    return prev;
}

void reorderList(ListNode *head) {
    if (head == NULL || head->next == NULL)
        return;

    ListNode *mid = find_mid(head);
    ListNode *second_half = mid->next;
    mid->next = NULL;

    second_half = reverse_list(second_half);

    ListNode *c1 = head, *n1 = head->next;
    ListNode *c2 = second_half;

    while (c1 != NULL && c2 != NULL) {
        ListNode *n2 = c2->next;
        c1->next = c2;
        c2->next = n1;
        c1 = n1;
        if (c1 != NULL)
            n1 = c1->next;
        c2 = n2;
    }
}
