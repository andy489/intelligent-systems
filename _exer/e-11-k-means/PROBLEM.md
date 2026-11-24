# Problem — K-Means Clustering

## Statement

Use the **K-Means** algorithm to automatically discover groups of similar elements
(clusters) for the datasets **normal.txt** and **unbalance.txt**.

**Input:** filename and number of clusters K.

**Output:** A scatter plot showing each cluster in a distinct colour, with centroids
marked visually.

---

## Datasets

Both datasets are 2-D — each point has an `x` and `y` coordinate.

| File | Points | Description |
|------|--------|-------------|
| `Datasets/normal/normal.txt` | 100 | Synthetic, N=100, overlapping clusters |
| `Datasets/unbalance/unbalance.txt` | 6500 | Synthetic, N=6500, M=8 Gaussian clusters |

---

## Requirements

| Level | Requirement |
|-------|-------------|
| Core | K-Means: assign points to nearest centroid, recalculate centroids, repeat until convergence |
| Core | Random Restart: run K-Means multiple times; keep the best result |
| Core | Evaluate quality with: **inner-cluster distance**, **inter-cluster distance**, and a **combination** of both |
| ★ | Apply **K-Means++** (smart centroid initialisation) and compare results |
| ★★ | Apply **Soft K-Means** (probabilistic assignments) and compare results |

---

## Formal Definition

Given N points and a chosen K, K-Means minimises the
**Within-Cluster Sum of Squares (WCSS)**:

```
WCSS = Σᵢ ||xᵢ − μ_{cᵢ}||²
```

where `μ_{cᵢ}` is the centroid of the cluster assigned to point `xᵢ`.

### Algorithm (Lloyd's)

```
1. Initialise K centroids
2. Assign each point to its nearest centroid
3. Recompute each centroid as the mean of its assigned points
4. Repeat 2–3 until centroids do not move (or max iterations)
```

### K-Means++ Initialisation

Replaces step 1 with a probabilistic spread:

```
1. Pick first centroid uniformly at random from the data
2. For each remaining centroid:
   - For every point compute D(x) = distance to nearest existing centroid
   - Choose next centroid with probability proportional to D(x)²
3. Proceed with standard K-Means
```

Guarantees O(log K) approximation to optimal WCSS.

---

## Quality Metrics

| Metric | Formula | Goal |
|--------|---------|------|
| WCSS (inner) | Σ dist²(point, its centroid) | Minimise |
| Min inter-centroid | min_{i≠j} dist²(cᵢ, cⱼ) | Maximise |
| Dunn index | min inter-centroid / max intra diameter | Maximise |

**Dunn index** combines both: a high value means clusters are far apart
*and* compact internally.

---

## Random Restart

Because K-Means converges to a local optimum, we run it R times with
different initialisations and keep the result with the best metric score.

| Parameter | Value |
|-----------|-------|
| Restarts R | 100 |
| Max iterations per run | 300 |
| Convergence threshold | 10⁻⁹ (squared centroid displacement) |
