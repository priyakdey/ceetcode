/**
 * @number: 48
 * @title: Rotate Image
 * @difficulty: Medium
 * @tags: array, math, matrix
 * @link: https://leetcode.com/problems/rotate-image/description
 */

#include <assert.h>
#include <stdlib.h>

void rotate(int **matrix, int matrixSize, int *matrixColSize) {
    assert(matrix != NULL && matrixSize > 0);
    int rows = matrixSize;
    int cols = matrixColSize[0];

    for (int row = 0; row < rows; row++) {
        for (int col = row; col < cols; col++) {
            int temp = matrix[row][col];
            matrix[row][col] = matrix[col][row];
            matrix[col][row] = temp;
        }
    }

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols / 2; col++) {
            int temp = matrix[row][col];
            matrix[row][col] = matrix[row][cols - 1 - col];
            matrix[row][cols - 1 - col] = temp;
        }
    }
}
