## Approach

Since we must buy before we sell, the best profit on any given day is that day's price
minus the lowest price seen on or before it. So we sweep left to right, treating day `0`
as the initial buy price, and track the running minimum:

- price is not higher than `buy_price` → this is a cheaper buy, update `buy_price`
- price is higher → a sell here yields `price - buy_price`; keep it if it beats
  `max_profit`

Every day is considered as a potential sell against the cheapest buy that precedes it,
so the single pass covers all valid buy/sell pairs. If prices only fall, no sell ever
improves on `0`, which is the correct "do nothing" answer.

## Complexity

- Time: `O(n)`
- Space: `O(1)`
