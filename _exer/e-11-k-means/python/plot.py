#!/usr/bin/env python3
"""
Render k-means clustering results produced by k_means (C++).

Usage:
    python3 plot.py <result.csv> [title] [output.png]

The CSV format expected (written by k_means):
    type,x,y,cluster
    point,1.2,3.4,0
    centroid,1.5,3.6,0
    ...
"""
import sys
import csv
import pathlib
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

PALETTE = [
    "#e6194b", "#3cb44b", "#4363d8", "#f58231",
    "#911eb4", "#42d4f4", "#f032e6", "#bfef45",
    "#fabed4", "#469990", "#dcbeff", "#9A6324",
]

def load(path: str):
    points, centroids = [], []
    with open(path) as f:
        reader = csv.DictReader(f)
        for row in reader:
            entry = (float(row["x"]), float(row["y"]), int(row["cluster"]))
            if row["type"] == "point":
                points.append(entry)
            else:
                centroids.append(entry)
    return points, centroids

def render(csv_path: str, title: str, out_path: str) -> None:
    points, centroids = load(csv_path)
    K = max(c[2] for c in centroids) + 1

    fig, ax = plt.subplots(figsize=(12, 7), dpi=130)
    ax.set_facecolor("#f8f8f8")
    fig.patch.set_facecolor("white")

    # Scatter points
    for x, y, k in points:
        ax.scatter(x, y, s=8, color=PALETTE[k % len(PALETTE)], alpha=0.65,
                   linewidths=0, zorder=2)

    # Centroids — bold hexagon markers
    for x, y, k in centroids:
        ax.plot(x, y, marker="h", markersize=16,
                color=PALETTE[k % len(PALETTE)],
                markeredgecolor="#333", markeredgewidth=1.5,
                zorder=5)

    # Legend
    patches = [mpatches.Patch(color=PALETTE[k % len(PALETTE)], label=f"Cluster {k}")
               for k in range(K)]
    ax.legend(handles=patches, loc="upper right", fontsize=8,
              framealpha=0.85, edgecolor="#ccc")

    ax.set_title(title, fontsize=14, fontweight="bold", pad=10)
    ax.set_xlabel("x"); ax.set_ylabel("y")
    ax.grid(True, linewidth=0.4, color="#ddd", zorder=0)

    plt.tight_layout()
    plt.savefig(out_path, bbox_inches="tight")
    plt.close()
    print(f"Saved → {out_path}")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print(__doc__)
        sys.exit(1)

    csv_path = sys.argv[1]
    title    = sys.argv[2] if len(sys.argv) > 2 else pathlib.Path(csv_path).stem
    out_path = sys.argv[3] if len(sys.argv) > 3 else pathlib.Path(csv_path).with_suffix(".png")
    render(csv_path, title, str(out_path))
