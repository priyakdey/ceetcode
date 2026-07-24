/**
 * @number: 124
 * @title: Binary Tree Maximum Path Sum
 * @difficulty: Hard
 * @tags: dynamic programming, tree, binary tree, dfs
 * @link: https://leetcode.com/problems/binary-tree-maximum-path-sum
 */

#include <limits.h>
#include <stdarg.h>

#include <sys/param.h>

typedef struct TreeNode TreeNode;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
};

int max_int(int count, ...) {
    va_list args;

    va_start(args, count);

    int max_value = va_arg(args, int);

    for (int i = 1; i < count; i++) {
        int current = va_arg(args, int);
        max_value = MAX(max_value, current);
    }

    va_end(args);

    return max_value;
}

int maxPathSumInternal(TreeNode *node, int *max_sum) {
    if (node->left == NULL && node->right == NULL) {
        *max_sum = max_int(2, *max_sum, node->val);
        return node->val;
    }

    int left_sum = 0, right_sum = 0;

    if (node->left != NULL)
        left_sum = maxPathSumInternal(node->left, max_sum);

    if (node->right != NULL)
        right_sum = maxPathSumInternal(node->right, max_sum);

    *max_sum = max_int(5, *max_sum, node->val, node->val + left_sum,
                       node->val + right_sum, node->val + left_sum + right_sum);

    return max_int(3, node->val, node->val + left_sum, node->val + right_sum);
}

int maxPathSum(TreeNode *root) {
    int max_sum = INT_MIN;

    maxPathSumInternal(root, &max_sum);
    return max_sum;
}
