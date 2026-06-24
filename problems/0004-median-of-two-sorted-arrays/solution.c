/**
 * @number: 4
 * @title: Median of Two Sorted Arrays
 * @difficulty: Hard
 * @tags: array, binary-search
 * @link: https://leetcode.com/problems/median-of-two-sorted-arrays/description/
 * @draft: true
 */


double findMedianOfSortedArray (int* nums, int numsSize) {
    int mid = numsSize / 2;
    if (numsSize % 2 == 0) {
        return ((double)nums[mid] + nums[mid - 1]) / 2;
    }

    return nums[mid];
}

double findMedianSortedArrays (int* nums1, int nums1Size, int* nums2, int nums2Size) {
    if (nums1Size == 0 && nums2Size == 0)
        return 0;
    if (nums1Size == 0)
        return findMedianOfSortedArray (nums2, nums2Size);
    if (nums2Size == 0)
        return findMedianOfSortedArray (nums1, nums1Size);

    int mid = (nums1Size + nums2Size) / 2;
    int i = 1, j = 0, curr = 0;
    int midElement = nums1[0], prevElement = -1;

    if (nums1[0] > nums2[0]) {
        i          = 0;
        j          = 1;
        midElement = nums2[0];
    }


    while (i < nums1Size && j < nums2Size && curr < mid) {
        prevElement = midElement;
        if (nums1[i] <= nums2[j]) {
            midElement = nums1[i++];
        } else {
            midElement = nums2[j++];
        }
        curr++;
    }

    while (i < nums1Size && curr < mid) {
        prevElement = midElement;
        midElement  = nums1[i++];
        curr++;
    }

    while (j < nums2Size && curr < mid) {
        prevElement = midElement;
        midElement  = nums2[j++];
        curr++;
    }

    if ((nums1Size + nums2Size) % 2 == 0) {
        return ((double)midElement + prevElement) / 2;
    }

    return midElement;
}
