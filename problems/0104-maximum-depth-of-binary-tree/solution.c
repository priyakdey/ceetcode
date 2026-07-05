/**
 * @number: 104
 * @title: Maximum Depth of Binary Tree
 * @difficulty: Easy
 * @tags: tree, bfs, dfs, binary-tree
 * @link: https://leetcode.com/problems/maximum-depth-of-binary-tree
 */

#include <sys/param.h>

typedef struct TreeNode TreeNode;

struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

int maxDepth(struct TreeNode *root) {
    if (root == NULL)
        return 0;

    int left = maxDepth(root->left);
    int right = maxDepth(root->right);

    return 1 + MAX(left, right);
}
