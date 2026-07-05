/**
 * @number: 100
 * @title: Same Tree
 * @difficulty: Easy
 * @tags: tree, bfs, dfs, binary-tree
 * @link: https://leetcode.com/problems/same-tree/description
 */

#include <stdbool.h>
#include <stdlib.h>

typedef struct TreeNode TreeNode;

struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

bool isSameTree(TreeNode *p, TreeNode *q) {
    if (p == NULL && q == NULL)
        return true;

    if (p == NULL || q == NULL)
        return false;
    if (p->val != q->val)
        return false;

    return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
}
