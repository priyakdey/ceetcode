/**
 * @number: 19 
 * @title: Remove Nth Node From End of List 
 * @difficulty: Medium
 * @tags: linked-list, two-pointers 
 * @link: https://leetcode.com/problems/remove-nth-node-from-end-of-list/description 
 */

#include <assert.h>
#include <stdlib.h>


// Definition for singly-linked list
struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* removeNthFromEnd(struct ListNode* head, int n) {
    struct ListNode dummy;
    dummy.next = head;

    struct ListNode *fast = head;
    struct ListNode *slow = &dummy;

    while (n > 0) {
        assert(fast != NULL);
        fast = fast->next;
        n--;
    }

    while (fast != NULL) {
        fast = fast->next;
        slow = slow->next;
    }

    slow->next = slow->next->next;
    head = dummy.next;
    return head;
}
