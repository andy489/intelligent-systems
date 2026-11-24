# Problem — Nearest Neighbour Search via k-d Tree

## Statement

Given **N** points in 2D space, build a **k-d tree** over them.  
Then answer **Q** nearest-neighbour queries: for each query point, find the
training point closest to it by Euclidean distance.

## Formal Definition

| Component | Definition |
|-----------|------------|
| **Point set** | N points `pᵢ = (xᵢ, yᵢ)`, indexed `0 … N−1` |
| **k-d tree** | A binary tree that partitions the 2D plane by alternating axis-aligned splits |
| **Splitting axis** | `depth % 2` — axis 0 (x) at even depth, axis 1 (y) at odd depth |
| **Split value** | Median of the points along the splitting axis |
| **Nearest neighbour** | The point `pᵢ` minimising `dist(pᵢ, q) = √((xᵢ−qx)² + (yᵢ−qy)²)` for query `q` |
| **Tie-breaking** | If two points are equidistant, return the one with the lower index |

## Input / Output

**Input format:**
```
N
x0 y0
x1 y1
…
x(N-1) y(N-1)
Q
qx0 qy0
qx1 qy1
…
qx(Q-1) qy(Q-1)
```

**Output:** One line per query:
```
Nearest to (qx, qy): point i (xi, yi) dist=<distance>
```
Distance is printed to **4 decimal places**.  Coordinates are printed to **2 decimal places**.

## Example

**Input:**
```
5
2.0 3.0
5.0 4.0
9.0 1.0
4.0 7.0
8.0 8.0
3
1.0 2.0
9.0 3.0
5.0 5.0
```

**Output:**
```
Nearest to (1.00, 2.00): point 0 (2.00, 3.00) dist=1.4142
Nearest to (9.00, 3.00): point 2 (9.00, 1.00) dist=2.0000
Nearest to (5.00, 5.00): point 1 (5.00, 4.00) dist=1.0000
```

**Tree built from the 5 points (root splits on x, median x = 5.0):**
```
              (5,4) [idx=1, split x=5]
             /                        \
      (2,3) [idx=0, split y=3]    (9,1) [idx=2, split y=1]
             \                        \
        (4,7) [idx=3]            (8,8) [idx=4]
```

## Special Cases

| Condition | Expected behaviour |
|-----------|-------------------|
| N = 1 | Every query returns the single point |
| Q = 0 | No output lines produced |
| Duplicate points | Return the one with the lower index |
| Query coincides with a point | Distance = 0.0000, return that point |
