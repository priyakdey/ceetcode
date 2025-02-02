// 2. Add Two Numbers
// https://leetcode.com/problems/add-two-numbers/description/

#include <assert.h>
#include <stdlib.h>

typedef struct ListNode ListNode;

struct ListNode
{
    int val;
    ListNode *next;
};

struct ListNode *addTwoNumbers(struct ListNode *l1, struct ListNode *l2)
{
    if (l1 == NULL && l2 == NULL)
    {
        return NULL;
    }

    if (l1 == NULL)
    {
        return l2;
    }

    if (l2 == NULL)
    {
        return l1;
    }

    ListNode *curr1 = l1, *tail = NULL;
    ListNode *curr2 = l2;
    int carry = 0;

    while (curr1 != NULL && curr2 != NULL)
    {
        int val = curr1->val + curr2->val + carry;
        carry = val / 10;
        val = val % 10;
        curr1->val = val;
        tail = curr1;
        curr1 = curr1->next;
        curr2 = curr2->next;
    }

    while (curr1 != NULL)
    {
        int val = curr1->val + carry;
        carry = val / 10;
        val = val % 10;
        curr1->val = val;
        tail = curr1;
        curr1 = curr1->next;
    }

    while (curr2 != NULL)
    {
        int val = curr2->val + carry;
        carry = val / 10;
        val = val % 10;
        ListNode *new_node = (ListNode *)malloc(sizeof(ListNode));
        assert(new_node != NULL);
        new_node->val = val;
        tail->next = new_node;
        tail = tail->next;
        curr2 = curr2->next;
    }

    if (carry == 1)
    {
        ListNode *new_node = (ListNode *)malloc(sizeof(ListNode));
        assert(new_node != NULL);
        new_node->val = 1;
        new_node->next = NULL;
        tail->next = new_node;
    }

    return l1;
}
