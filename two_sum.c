// 1. Two Sum
// https://leetcode.com/problems/two-sum/description/

#include <assert.h>
#include <stdlib.h>

typedef struct Node Node;

struct Node
{
    int key;
    int value;
    Node *next;
};

typedef struct
{
    Node **table;
    size_t capacity;
} HashTable;

HashTable *ht_create(size_t capacity)
{
    HashTable *ht = malloc(sizeof(HashTable));
    assert(ht != NULL);
    ht->table = malloc(sizeof(Node *) * capacity);
    assert(ht->table != NULL);
    ht->capacity = capacity;
    for (size_t i = 0; i < capacity; i++)
    {
        ht->table[i] = NULL;
    }
    return ht;
}

void ht_put(HashTable *ht, int key, int value)
{
    size_t index = ((key % ht->capacity) + ht->capacity) % ht->capacity;
    Node *node = ht->table[index];

    if (node == NULL)
    {
        node = (Node *)malloc(sizeof(Node));
        assert(node != NULL);
        node->key = key;
        node->value = value;
        node->next = NULL;
        ht->table[index] = node;
        return;
    }

    while (node->next != NULL && node->key != key)
    {
        node = node->next;
    }

    if (node->key == key)
    {
        node->value = value;
        return;
    }

    Node *new_node = (Node *)malloc(sizeof(Node));
    assert(new_node != NULL);
    new_node->key = key;
    new_node->value = value;
    new_node->next = NULL;
    node->next = new_node;
    return;
}

int *ht_get(HashTable *ht, int key)
{
    size_t index = ((key % ht->capacity) + ht->capacity) % ht->capacity;
    Node *node = ht->table[index];

    if (node == NULL)
    {
        return NULL;
    }

    while (node != NULL)
    {
        if (node->key == key)
        {
            return &node->value;
        }
    }

    return NULL;
}

int *twoSum(int *nums, int numsSize, int target, int *returnSize)
{

    HashTable *ht = ht_create((size_t)numsSize);

    for (int i = 0; i < numsSize; i++)
    {
        int num = nums[i];
        int compliment = target - num;
        int *value = ht_get(ht, compliment);
        if (value != NULL)
        {
            int *result = (int *)malloc(sizeof(int) * 2);
            result[0] = *value;
            result[1] = i;
            *returnSize = 2;
            return result;
        }

        ht_put(ht, num, i);
    }

    return NULL;
}