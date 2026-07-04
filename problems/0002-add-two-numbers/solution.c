/**
 * @number: 2
 * @title: Add Two Numbers
 * @difficulty: Medium
 * @tags: linkedlist, math, recursion
 * @link: https://leetcode.com/problems/add-two-numbers/description/
 */

#include <assert.h>
#include <stdlib.h>

typedef struct ListNode ListNode;

struct ListNode {
    int val;
    ListNode *next;
};

/// NOTE: The leetcode solution uses `struct ListNode` in the stub.
/// We have forward declared by type in this solution. If you wanna
/// this in LeetCode, adapt the solution accordingly.
ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
    if (l1 == NULL && l2 == NULL)
        return NULL;
    if (l1 == NULL)
        return l2;
    if (l2 == NULL)
        return l1;

    ListNode *curr1 = l1, *curr2 = l2;
    ListNode dummy = {-1, NULL};
    ListNode *tail = &dummy;
    int carry = 0;

    while (curr1 || curr2 || carry) {
        int curr1Val = curr1 != NULL ? curr1->val : 0;
        int curr2Val = curr2 != NULL ? curr2->val : 0;

        int val = curr1Val + curr2Val + carry;
        carry = val / 10;
        val = val % 10;

        ListNode *node = (ListNode *)malloc(sizeof(ListNode));
        assert(node != NULL);

        node->val = val;
        node->next = NULL;
        tail->next = node;
        tail = node;

        if (curr1)
            curr1 = curr1->next;
        if (curr2)
            curr2 = curr2->next;
    }

    return dummy.next;
}
