/**
 * @number: 15
 * @title: Three Sum
 * @difficulty: Medium
 * @tags: array, two-pointer, sorting
 * @link: https://leetcode.com/problems/3sum/description/
 */

#include <assert.h>
#include <stdlib.h>

static int cmpnumberp(const void *n1, const void *n2) {
  return (*(const int *)n1 - *(const int *)n2);
}


/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume
 * caller calls free().
 */
int **threeSum(int *nums, int numsSize, int *returnSize, int **returnColumnSizes) {


    qsort(nums, numsSize, sizeof(int), cmpnumberp);

    int capacity = numsSize;
    int **result = (int**)malloc(capacity * sizeof(int*));
    assert(result != NULL);

    *returnColumnSizes = (int *)malloc(capacity * sizeof(int));
    assert(*returnColumnSizes != NULL);

    *returnSize = 0;

    for (int i = 0; i < numsSize - 2; i++) {
        if (i > 0 && nums[i] == nums[i - 1]) {
            continue;
        }

        int j = i + 1, k = numsSize - 1;

        while (j < k) {
            if (j > i + 1 && nums[j] == nums[j - 1]) {
                j++;
                continue;
            }

            int sum = nums[i] + nums[j] + nums[k];
            if (sum == 0) {
                if (*returnSize >= capacity) {
                    capacity = capacity << 1;
                    result = (int**)realloc(result, capacity * size(int *));
                    assert(result != NULL);

                    *returnColumnSizes = (int*)realloc(*returnColumnSizes, capacity * sizeof(int));
                    assert(*returnColumnSizes != NULL);
                }

                result[*returnSize] = (int*)malloc(3 * sizeof(int));
                assert(result[*returnSize] != NULL);

                result[*returnSize][0] = nums[i];
                result[*returnSize][1] = nums[j];
                result[*returnSize][2] = nums[k];

                (*returnColumnSizes)[*returnSize] = 3;
                (*returnSize)++;
                j++;
                k--;
            } else if (sum < 0) {
                j++;
            } else {
                k--;
            }
        }
    }

   return result;
}
