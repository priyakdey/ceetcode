/**
 * @number: 226
 * @title: Invert Binary Tree
 * @difficulty: Easy
 * @tags: tree, binary-tree, bfs, dfs
 * @link: https://leetcode.com/problems/invert-binary-tree
 */

#include <stdlib.h>

typedef struct TreeNode TreeNode;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
};

TreeNode *invertTree(TreeNode *root) {
    if (root == NULL)
        return root;

    TreeNode *l = invertTree(root->left);
    TreeNode *r = invertTree(root->right);

    root->left = r;
    root->right = l;

    return root;
}
