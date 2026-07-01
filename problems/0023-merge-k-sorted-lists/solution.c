/**
 * @number: 23
 * @title: Merge k Sorted Lists
 * @difficulty: Hard
 * @tags: linked-list, divide-and-conquer, heap, merge-sort
 * @link: https://leetcode.com/problems/merge-k-sorted-lists/
 */
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct ListNode ListNode;

/// Definition of a single linked list node
struct ListNode {
    int val;
    struct ListNode *next;
};

int get_left_child_index(size_t p_index) { return 2 * p_index + 1; }
int get_right_child_index(size_t p_index) { return 2 * p_index + 2; }
int get_parent_index(size_t c_index) { return (c_index - 1) / 2; }

typedef struct {
    ListNode **data;
    size_t size;
} PriorityQueue;

PriorityQueue *pq_init(size_t capacity) {
    PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));
    pq->data = (ListNode **)malloc(sizeof(ListNode *) * capacity);
    pq->size = 0;
    return pq;
}

void pq_swap(PriorityQueue *pq, size_t i, size_t j) {
    ListNode *temp = pq->data[i];
    pq->data[i] = pq->data[j];
    pq->data[j] = temp;
}

void pq_push(PriorityQueue *pq, ListNode *node) {
    pq->data[pq->size] = node;
    pq->size++;

    size_t curr = pq->size - 1;

    while (curr > 0) {
        size_t parent_index = get_parent_index(curr);
        if (pq->data[curr]->val < pq->data[parent_index]->val) {
            pq_swap(pq, parent_index, curr);
            curr = parent_index;
        } else {
            break;
        }
    }
}

ListNode *pq_pop(PriorityQueue *pq) {
    assert(pq->size > 0);
    ListNode *head = pq->data[0];
    pq_swap(pq, 0, pq->size - 1);

    pq->size--;
    size_t curr = 0;

    while (curr < pq->size) {
        size_t left_index = get_left_child_index(curr);

        if (left_index >= pq->size) {
            break;
        }

        size_t right_index = get_right_child_index(curr);

        size_t swap = left_index;
        if (right_index < pq->size &&
            pq->data[right_index]->val < pq->data[left_index]->val) {
            swap = right_index;
        }

        if (pq->data[swap]->val < pq->data[curr]->val) {
            pq_swap(pq, curr, swap);
            curr = swap;
        } else {
            break;
        }
    }

    return head;
}

bool pq_empty(PriorityQueue *pq) { return pq->size == 0; }

struct ListNode *mergeKLists(struct ListNode **lists, int listsSize) {

    PriorityQueue *pq = pq_init(listsSize);

    for (int i = 0; i < listsSize; i++) {
        ListNode *node = lists[i];
        if (node != NULL) {
            pq_push(pq, node);
        }
    }

    ListNode *dummy = (ListNode *)malloc(sizeof(ListNode) * 1);
    dummy->next = NULL;
    ListNode *curr = dummy;

    while (!pq_empty(pq)) {
        ListNode *node = pq_pop(pq);
        curr->next = node;
        curr = curr->next;

        if (node->next != NULL) {
            pq_push(pq, node->next);
        }
    }

    ListNode *result = dummy->next;
    free(dummy);
    free(pq->data);
    free(pq);

    return result;
}
