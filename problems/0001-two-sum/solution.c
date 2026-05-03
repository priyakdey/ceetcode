/**
 * @number: 1
 * @title: Two Sum
 * @difficulty: Easy
 * @tags: array, hashtable 
 * @link: https://leetcode.com/problems/two-sum/description/
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node Node;

typedef struct Node
{
    int  key;
    int  hash;
    int  value;
    Node *next;
};

typedef struct
{
    Node   **table;
    size_t capacity;
} HashTable;

HashTable *create_table(size_t capacity)
{
    HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
    if (ht == NULL)
    {
        fprintf(stderr, "ERROR: could not allocate memory for hashtable\n");
        exit(1);
    }

    ht->table = (Node **)malloc(sizeof(Node *) * capacity);
    if (ht == NULL)
    {
        fprintf(stderr, "ERROR: could not allocate memory for hashtable\n");
        exit(1);
    }

    ht->capacity = capacity;
    memset(ht->table, '\0', capacity * sizeof(Node *));
    return ht;
}

void put_key(HashTable *ht, int key, int value)
{
    int hash = key;
    int index = hash % ht->capacity;

    if (ht->table[index] == NULL)
    {
        Node *new_node = (Node *)malloc(sizeof(Node));
        if (new_node == NULL)
        {
            fprintf(stderr, "ERROR: could not allocate memory for node\n");
            exit(1);
        }
        new_node->key = key;
        new_node->hash = hash;
        new_node->value = value;
        new_node->next = NULL;
        ht->table[index] = new_node;
        return;
    }

    Node *node = ht->table[index];
    Node *tail = NULL;
    while (node != NULL && node->key != key)
    {
        tail = node;
        node = node->next;
    }

    if (node == NULL)
    {
        Node *new_node = (Node *)malloc(sizeof(Node));
        if (new_node == NULL)
        {
            fprintf(stderr, "ERROR: could not allocate memory for node\n");
            exit(1);
        }

        new_node->key = key;
        new_node->hash = hash;
        new_node->value = value;
        new_node->next = NULL;
        tail->next = new_node;
    }
    else
    {
        node->value = value;
    }
}

bool ht_get(HashTable *ht, int key, int *value)
{
    int hash = key;
    int index = hash % ht->capacity;

    Node *node = ht->table[index];
    while (node != NULL && node->key != key)
    {
        node = node->next;
    }

    if (node == NULL)
    {
        return false;
    }

    *value = node->value;
    return true;
}

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int *twoSum(int *nums, int numsSize, int target, int *returnSize)
{
    // numSize may not be a great choice for number of buckets
    // We could would be to clamp to next greatest power to 2 
    // but it works for this leetcode style program, and we do not need to
    // worry about resizing the table.
    size_t cap = numsSize * 2;
    HashTable *ht = create_table(cap);
    for (int i = 0; i < numsSize; i++)
    {
        int num = nums[i];
        int complement = target - num;
        int index;
        if (ht_get(ht, complement, &index))
        {
            int *result = (int *)malloc(sizeof(int) * 2);
            result[0] = index;
            result[1] = i;
            *returnSize = 2;
            return result;
        }
        put_key(ht, num, i);
    }

    *returnSize = 0;
    return NULL;
}

