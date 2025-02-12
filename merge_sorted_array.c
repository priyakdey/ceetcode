// 88. Merge Sorted Array
// https://leetcode.com/problems/merge-sorted-array/

void merge(int *nums1, int nums1Size, int m, int *nums2, int nums2Size, int n)
{
    int cursor = nums1Size - 1;
    int i = m - 1, j = n - 1;

    while (i >= 0 && j >= 0)
    {
        if (nums1[i] >= nums2[j])
        {
            nums1[cursor--] = nums1[i--];
        }
        else
        {
            nums1[cursor--] = nums2[j--];
        }
    }

    while (j >= 0)
    {
        nums1[cursor--] = nums2[j--];
    }
}