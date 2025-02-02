// 14. Longest Common Prefix
// https://leetcode.com/problems/longest-common-prefix/description/

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_CAPACITY 26

typedef struct Node Node;

struct Node
{
    Node **table;
    bool is_end;
};

typedef struct
{
    Node *root;
} Trie;

Node *node_create()
{
    Node *node = (Node *)malloc(sizeof(Node));
    assert(node != NULL);
    node->table = (Node **)malloc(sizeof(Node *) * TABLE_CAPACITY);
    for (int i = 0; i < TABLE_CAPACITY; i++)
    {
        node->table[i] = NULL;
    }
    node->is_end = false;
    return node;
}

Trie *trie_create()
{
    Trie *t = (Trie *)malloc(sizeof(Trie));
    assert(t != NULL);
    t->root = node_create();
    return t;
}

void trie_insert(Trie *t, char *word, size_t word_size)
{
    Node *curr = t->root;

    for (size_t i = 0; i < word_size; i++)
    {
        size_t index = word[i] - 'a';
        if (curr->table[index] == NULL)
        {
            curr->table[index] = node_create();
        }
        curr = curr->table[index];
    }

    curr->is_end = true;
}

char *trie_find_largest_common_prefix(const Trie *t)
{
    // constraints: 0 <= strs[i].length <= 200
    char *buf = (char *)malloc(sizeof(char) * (200 + 1));

    int cursor = 0;
    Node *curr = t->root;

    while (!curr->is_end && curr != NULL)
    {
        int index;
        int count = 0;
        for (int i = 0; i < TABLE_CAPACITY; i++)
        {
            if (curr->table[i])
            {
                index = i;
                count++;
            }

            if (count == 2)
            {
                break;
            }
        }

        if (count == 0 || count > 1)
        {
            break;
        }

        char ch = index + 'a';
        buf[cursor++] = ch;
        curr = curr->table[index];
    }

    buf[cursor++] = '\0';
    return buf;
}

char *longestCommonPrefix(char **strs, int strsSize)
{
    Trie *t = trie_create();
    for (int i = 0; i < strsSize; i++)
    {
        trie_insert(t, strs[i], strlen(strs[i]));
    }

    return trie_find_largest_common_prefix(t);
}