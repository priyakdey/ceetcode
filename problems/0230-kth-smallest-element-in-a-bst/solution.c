/**
 * @number: 230
 * @title: Kth Smallest Element in a BST
 * @difficulty: Medium
 * @tags: tree, dfs, bst, binary tree
 * @link: https://leetcode.com/problems/kth-smallest-element-in-a-bst
 */

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct TreeNode TreeNode;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
};

typedef struct {
    TreeNode **nodes;
    uint32_t size;
    uint32_t capacity;
} Stack;

Stack *stack_init(uint32_t capacity) {
    Stack *stack = (Stack *)calloc(sizeof(Stack), 1);
    TreeNode **nodes = (TreeNode **)calloc(sizeof(TreeNode *), capacity);

    stack->nodes = nodes;
    stack->size = 0;
    stack->capacity = capacity;

    return stack;
}

void resize(Stack *stack) {
    stack->capacity = stack->capacity << 1;

    TreeNode **nodes = (TreeNode **)calloc(sizeof(TreeNode *), stack->capacity);

    for (int i = 0; i < stack->size; i++) {
        nodes[i] = stack->nodes[i];
    }

    stack->nodes = nodes;
}

void stack_push(Stack *stack, TreeNode *node) {
    if (stack->size == stack->capacity) {
        resize(stack);
    }

    stack->nodes[stack->size++] = node;
}

TreeNode *stack_pop(Stack *stack) {
    assert(stack->size > 0);

    TreeNode *node = stack->nodes[stack->size - 1];
    stack->size--;

    return node;
}

int kthSmallest(struct TreeNode *root, int k) {
    Stack *stack = stack_init(k);

    TreeNode *node = root;
    while (node != NULL) {
        stack_push(stack, node);
        node = node->left;
    }

    while (stack->size > 0) {
        node = stack_pop(stack);
        k--;

        if (k == 0)
            return node->val;

        node = node->right;
        while (node != NULL) {
            stack_push(stack, node);
            node = node->left;
        }
    }

    assert("UNREACHABLE" && 0);
}
