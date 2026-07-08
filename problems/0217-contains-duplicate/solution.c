/**
 * @number: 217
 * @title: Contains Duplicate
 * @difficulty: Easy
 * @tags: array, hash-table, sorting
 * @link: https://leetcode.com/problems/contains-duplicate
 */

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define INIT_CAPACITY 1 << 16
#define DEFAULT_LOAD_FACTOR 0.7

typedef struct Node Node;

struct Node {
    int val;
    Node *next;
};

typedef struct {
    uint64_t capacity;
    uint64_t size;
    float load_capacity;
    Node **buckets;
} Set;

Set *init_set(uint64_t capacity, float load_capcity) {
    assert(capacity > 0);
    assert(load_capcity > 0 && load_capcity <= 1);

    Set *set = (Set *)calloc(1, sizeof(Set));
    set->capacity = capacity;
    set->size = 0;
    set->load_capacity = load_capcity;
    set->buckets = (Node **)calloc(capacity, sizeof(Node *));
    return set;
}

void add_set(Set *set, int val) {
    if (set->size >= set->capacity * set->load_capacity) {
        // TODO: realloc the bucket
    }

    int index = val % set->capacity;

    if (set->buckets[index] == NULL) {
        set->buckets[index] = (Node *)calloc(1, sizeof(Node));
    }

    Node *node = set->buckets[index];

    while (node->next != NULL) {
        node = node->next;
    }
    node->val = val;
    set->size++;
}

bool is_present_set(Set *set, int val) {
    int index = val % set->capacity;

    if (set->buckets[index] == NULL) {
        return false;
    }

    Node *node = set->buckets[index];

    while (node != NULL) {
        if (node->val == val)
            return true;
        node = node->next;
    }

    return false;
}

// NOTE: memory leak
bool containsDuplicate(int *nums, int numsSize) {
    Set *set = init_set(INIT_CAPACITY, DEFAULT_LOAD_FACTOR);

    for (int i = 0; i < numsSize; i++) {
        int val = nums[i];
        if (is_present_set(set, val)) {
            return true;
        }
        add_set(set, val);
    }

    return false;
}
