/**
 * @number: 121
 * @title: Best Time to Buy and Sell Stock
 * @difficulty: Easy
 * @tags: array, dynamic-programming
 * @link: https://leetcode.com/problems/best-time-to-buy-and-sell-stock
 */

#include <assert.h>

#include <sys/param.h>

int maxProfit(int *prices, int pricesSize) {
    assert(pricesSize > 0);

    int buy_price = prices[0];
    int max_profit = 0;

    for (int i = 1; i < pricesSize; i++) {
        int price = prices[i];
        if (price <= buy_price) {
            buy_price = price;
        } else {
            max_profit = MAX(max_profit, price - buy_price);
        }
    }

    return max_profit;
}
