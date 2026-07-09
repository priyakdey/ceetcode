/**
 * @number: 572
 * @title: Subtree of Another Tree
 * @difficulty: Easy
 * @tags: tree, dfs, bfs, string-matching, hash-function
 * @link: https://leetcode.com/problems/subtree-of-another-tree
 */

#include <stdbool.h>
#include <stdlib.h>

typedef struct TreeNode TreeNode;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
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

bool isSubtree(TreeNode *root, TreeNode *subRoot) {
    if (root == NULL)
        return false;

    return isSameTree(root, subRoot) || isSubtree(root->left, subRoot) ||
           isSubtree(root->right, subRoot);
}
