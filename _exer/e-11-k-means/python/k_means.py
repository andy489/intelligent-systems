import sys
import os
import math
import random
import csv

# ─── Constants ────────────────────────────────────────────────────────────────

RANDOM_RESTARTS       = 100
MAX_ITERATIONS        = 300
CENTROID_MOVE_EPSILON = 1e-9

# ─── I/O ──────────────────────────────────────────────────────────────────────

def read_points(path):
    pts = []
    with open(path) as f:
        for line in f:
            line = line.strip()
            if line:
                x, y = map(float, line.split())
                pts.append((x, y))
    return pts

def write_result(out_path, pts, labels, centroids):
    with open(out_path, "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["type", "x", "y", "cluster"])
        for (x, y), lbl in zip(pts, labels):
            w.writerow(["point", x, y, lbl])
        for k, (x, y) in enumerate(centroids):
            w.writerow(["centroid", x, y, k])

# ─── Distance ─────────────────────────────────────────────────────────────────

def sq_dist(a, b):
    return (a[0] - b[0]) ** 2 + (a[1] - b[1]) ** 2

# ─── Initialisations ──────────────────────────────────────────────────────────

def init_random(pts, K, rng):
    xs = [p[0] for p in pts]
    ys = [p[1] for p in pts]
    return [(rng.uniform(min(xs), max(xs)), rng.uniform(min(ys), max(ys)))
            for _ in range(K)]

def init_plus_plus(pts, K, rng):
    centroids = [rng.choice(pts)]
    d = [math.inf] * len(pts)
    for _ in range(1, K):
        for i, p in enumerate(pts):
            dist = sq_dist(p, centroids[-1])
            if dist < d[i]:
                d[i] = dist
        total = sum(d)
        r = rng.uniform(0, total)
        cumul = 0.0
        for i, w in enumerate(d):
            cumul += w
            if cumul >= r:
                centroids.append(pts[i])
                break
        else:
            centroids.append(pts[-1])
    return centroids

# ─── K-Means core ─────────────────────────────────────────────────────────────

def run_kmeans(pts, centroids):
    n = len(pts)
    K = len(centroids)
    labels = [0] * n

    for _ in range(MAX_ITERATIONS):
        # Assignment
        for i, p in enumerate(pts):
            labels[i] = min(range(K), key=lambda k: sq_dist(p, centroids[k]))

        # Update
        sx = [0.0] * K
        sy = [0.0] * K
        cnt = [0] * K
        for i, p in enumerate(pts):
            sx[labels[i]] += p[0]
            sy[labels[i]] += p[1]
            cnt[labels[i]] += 1

        moved = False
        new_centroids = list(centroids)
        for k in range(K):
            if cnt[k] == 0:
                continue
            nc = (sx[k] / cnt[k], sy[k] / cnt[k])
            if sq_dist(nc, centroids[k]) > CENTROID_MOVE_EPSILON:
                new_centroids[k] = nc
                moved = True
        centroids = new_centroids
        if not moved:
            break

    return labels, centroids

# ─── Quality metrics ──────────────────────────────────────────────────────────

def wcss(pts, labels, centroids):
    return sum(sq_dist(p, centroids[labels[i]]) for i, p in enumerate(pts))

def min_inter_centroid(centroids):
    K = len(centroids)
    return min(sq_dist(centroids[i], centroids[j])
               for i in range(K) for j in range(i + 1, K))

def max_intra_cluster_dist(pts, labels, centroids):
    K = len(centroids)
    max_r = [0.0] * K
    for i, p in enumerate(pts):
        k = labels[i]
        d = sq_dist(p, centroids[k])
        if d > max_r[k]:
            max_r[k] = d
    return max(max_r) * 4.0  # (2r)^2 = 4r^2

def dunn_index(pts, labels, centroids):
    inter = min_inter_centroid(centroids)
    intra = max_intra_cluster_dist(pts, labels, centroids)
    if intra < 1e-15:
        return math.inf
    return inter / intra

# ─── Random Restart ───────────────────────────────────────────────────────────

def random_restart(pts, K, metric, use_plus_plus, seed=42):
    rng = random.Random(seed)
    best_labels = None
    best_centroids = None
    best_score = math.inf if metric == "inner" else -math.inf

    for _ in range(RANDOM_RESTARTS):
        c0 = init_plus_plus(pts, K, rng) if use_plus_plus else init_random(pts, K, rng)
        labels, centroids = run_kmeans(pts, c0)

        if metric == "inner":
            score = wcss(pts, labels, centroids)
            better = score < best_score
        elif metric == "outer":
            score = min_inter_centroid(centroids)
            better = score > best_score
        else:
            score = dunn_index(pts, labels, centroids)
            better = score > best_score

        if better:
            best_score = score
            best_labels = labels
            best_centroids = centroids

    metric_name = {"inner": "WCSS (inner)", "outer": "min-inter-centroid", "both": "Dunn index"}[metric]
    init_name = "K-Means++" if use_plus_plus else "K-Means"
    print(f"[{init_name} | {metric_name}] best score = {best_score:.4f}")

    return best_labels, best_centroids

# ─── Main ─────────────────────────────────────────────────────────────────────

METRIC_MAP = {"1": "inner", "2": "outer", "3": "both"}

def main():
    if len(sys.argv) >= 3:
        data_file  = sys.argv[1]
        K          = int(sys.argv[2])
        metric     = "both"
        plus_plus  = True
    else:
        print("\033[36;1mK-Means Clustering\033[0m\n")
        data_file = input("Data file (e.g. ../datasets/normal/normal.txt): ").strip()
        K         = int(input("Number of clusters K: "))
        m         = input("Metric [1=WCSS(inner), 2=inter-centroid, 3=Dunn(both)]: ").strip()
        metric    = METRIC_MAP.get(m, "both")
        pp        = input("Init [1=random, 2=K-Means++]: ").strip()
        plus_plus = (pp != "1")

    pts = read_points(data_file)
    assert pts, "Empty dataset"
    print(f"Loaded {len(pts)} points, K={K}")

    labels, centroids = random_restart(pts, K, metric, plus_plus)

    out_csv = sys.argv[3] if len(sys.argv) >= 4 else \
        os.path.join(os.path.dirname(os.path.abspath(__file__)), "result.csv")
    write_result(out_csv, pts, labels, centroids)
    print(f"Result written to {out_csv}")

if __name__ == "__main__":
    main()
