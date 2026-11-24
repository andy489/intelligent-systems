# Solution — Nearest Neighbour Search via k-d Tree

## Algorithm Overview

A **k-d tree** (k-dimensional tree) is a binary space-partitioning tree that
organises points in k-dimensional space.  For 2D nearest-neighbour search it
reduces average query time from O(N) (brute force) to O(log N) by pruning
entire subtrees that cannot contain a closer point than the current best.

---

## Tree Construction

**Key idea:** recursively partition the point set by splitting along alternating
axes, always splitting at the median so the tree stays balanced.

```
build(points, depth):
    if points is empty → return null
    axis = depth % 2          // 0 = x-axis, 1 = y-axis
    sort points by axis
    mid = len(points) // 2    // lower median → left-heavy on even sizes
    node.point = points[mid]
    node.left  = build(points[:mid],   depth + 1)
    node.right = build(points[mid+1:], depth + 1)
    return node
```

Sorting at each level gives **O(N log N)** total build time (or O(N log² N)
with naive re-sorting; O(N log N) with pre-sorted arrays).

---

## Nearest Neighbour Search

**Key idea:** traverse the tree like a BST, tracking the best distance seen so
far.  After visiting the near subtree, only visit the far subtree if its
bounding half-plane could contain a closer point.

```
nearest(node, query, depth, best):
    if node is null → return

    // 1. Update best with this node
    d = dist(node.point, query)
    if d < best.dist: best = (node, d)

    // 2. Choose near / far child (same logic as BST lookup)
    axis = depth % 2
    diff = query[axis] − node.point[axis]
    near, far = (left, right) if diff <= 0 else (right, left)

    // 3. Always recurse into the near subtree
    nearest(near, query, depth+1, best)

    // 4. Only recurse into the far subtree if it could improve best
    //    The closest point in the far half-plane is |diff| away on the axis.
    if |diff| < best.dist:
        nearest(far, query, depth+1, best)
```

The pruning condition `|diff| < best.dist` is the core of the algorithm.
`diff` is the signed distance from the query point to the splitting hyperplane;
if the absolute value exceeds the current best distance, the entire far subtree
lies outside the hypersphere of radius `best.dist` around the query and can be
skipped entirely.

> **Common bug:** using `diff²` vs `bestDist²` for the pruning check is correct
> and avoids a sqrt, but mixing squared and unsquared values silently breaks the
> condition.  All three implementations here use plain (unsquared) distances for
> clarity.

---

## Complexity

| Operation | Time | Space |
|-----------|------|-------|
| Build tree | O(N log N) | O(N) |
| Single NN query — average | O(log N) | O(log N) stack |
| Single NN query — worst case | O(N) | O(N) stack |
| Q queries total — average | O(Q log N) | O(log N) |

**Worst case** arises when all points lie on a line perpendicular to the
splitting axis, forcing every subtree to be visited.  In practice, with
uniformly distributed 2D points, the average is O(log N).

---

## Why It Beats Brute Force — and When It Doesn't

| Scenario | Brute force | k-d tree |
|----------|-------------|----------|
| 2D, N = 10 000, Q = 10 000 | 10⁸ distance computations | ~10⁴ × 14 ≈ 10⁵·¹ |
| High-dimensional data (d ≫ 10) | O(N) | Degrades toward O(N) |

**Curse of dimensionality:** in high dimensions almost every point is "close"
to the splitting hyperplane, so the pruning condition `|diff| < bestDist`
rarely fires and nearly all subtrees must be visited.  For d ≳ 20, brute
force or approximate methods (e.g. ball trees, LSH) are preferred.

This is directly related to the **curse of dimensionality** mentioned in the
kNN lecture slides (E08): irrelevant features inflate distance, diluting the
signal from relevant ones.

---

## Source Files

| Language | File |
|----------|------|
| Java | `java/src/KdTree.java` |
| Python | `python/kd_tree.py` |
| C++ | `cpp/kd_tree.cpp` |

---

## Build & Run

Use the example from PROBLEM.md:

```bash
INPUT="5
2.0 3.0
5.0 4.0
9.0 1.0
4.0 7.0
8.0 8.0
3
1.0 2.0
9.0 3.0
5.0 5.0"
```

**Java**
```bash
cd kd-tree/java
javac src/KdTree.java -d out
echo "$INPUT" | java -cp out KdTree
```

**Python**
```bash
echo "$INPUT" | python3 kd-tree/python/kd_tree.py
```

**C++**
```bash
cd kd-tree/cpp
g++ -std=c++17 -O2 -o kd_tree kd_tree.cpp
echo "$INPUT" | ./kd_tree
```

Expected output for all three:
```
Nearest to (1.00, 2.00): point 0 (2.00, 3.00) dist=1.4142
Nearest to (9.00, 3.00): point 2 (9.00, 1.00) dist=2.0000
Nearest to (5.00, 5.00): point 1 (5.00, 4.00) dist=1.0000
```
