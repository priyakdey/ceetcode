/**
 * @number: 11
 * @title: Container with Most Water 
 * @difficulty: Medium 
 * @tags: array, two-pointers, greedy 
 * @link: https://leetcode.com/problems/container-with-most-water 
 */


#include <sys/param.h>


int maxArea(int* height, int heightSize)
{
    int left = 0, right = heightSize - 1;
    int maxArea = 0;

    while (left < right)
    {
        int boundingHeight = MIN(height[left], height[right]);
        int area = boundingHeight * (right - left);
        maxArea = MAX(maxArea, area);

        if (height[left] <= height[right])
        {
            left++;
        } else 
        {
            right--;
        }

    }

    return maxArea;
}
