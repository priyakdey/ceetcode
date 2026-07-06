/**
 * @number: 141
 * @title: Linked List Cycle
 * @difficulty: Easy
 * @tags: hash-table, linked-list, two-pointers
 * @link: https://leetcode.com/problems/linked-list-cycle
 */

#include <stdbool.h>
#include <stddef.h>

typedef struct ListNode ListNode;

struct ListNode {
    int val;
    struct ListNode *next;
};

bool hasCycle(ListNode *head) {
    if (head == NULL || head->next == NULL)
        return false;

    ListNode *slow = head;
    ListNode *fast = head;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast)
            break;
    }

    return slow == fast;
}
