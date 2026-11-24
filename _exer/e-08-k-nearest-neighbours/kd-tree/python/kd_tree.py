import sys
import math
from dataclasses import dataclass, field
from typing import Optional, List, Tuple

@dataclass
class Node:
    x: float
    y: float
    idx: int
    left: Optional['Node'] = field(default=None, repr=False)
    right: Optional['Node'] = field(default=None, repr=False)


def build(points: List[Tuple[float, float, int]], depth: int) -> Optional[Node]:
    if not points:
        return None
    axis = depth % 2
    points.sort(key=lambda p: p[axis])
    mid = len(points) // 2
    px, py, pidx = points[mid]
    node = Node(px, py, pidx)
    node.left  = build(points[:mid],    depth + 1)
    node.right = build(points[mid+1:],  depth + 1)
    return node


def nearest(node: Optional[Node], qx: float, qy: float, depth: int,
            best: List) -> None:
    """best is [dist, idx] updated in-place."""
    if node is None:
        return

    d = math.sqrt((node.x - qx) ** 2 + (node.y - qy) ** 2)
    if d < best[0] or (d == best[0] and node.idx < best[1]):
        best[0] = d
        best[1] = node.idx

    axis = depth % 2
    diff = (qx if axis == 0 else qy) - (node.x if axis == 0 else node.y)

    near = node.left  if diff <= 0 else node.right
    far  = node.right if diff <= 0 else node.left

    nearest(near, qx, qy, depth + 1, best)

    if abs(diff) < best[0]:
        nearest(far, qx, qy, depth + 1, best)


def main():
    data = sys.stdin.read().split()
    pos = 0

    n = int(data[pos]); pos += 1
    pts = []
    for i in range(n):
        x = float(data[pos]); pos += 1
        y = float(data[pos]); pos += 1
        pts.append((x, y, i))

    root = build(pts, 0)

    q = int(data[pos]); pos += 1
    # Rebuild flat list for output lookup
    point_coords = [(p[0], p[1]) for p in sorted(pts, key=lambda p: p[2])]

    for _ in range(q):
        qx = float(data[pos]); pos += 1
        qy = float(data[pos]); pos += 1

        best = [math.inf, -1]
        nearest(root, qx, qy, 0, best)

        idx = best[1]
        px, py = point_coords[idx]
        print(f"Nearest to ({qx:.2f}, {qy:.2f}): point {idx} ({px:.2f}, {py:.2f}) dist={best[0]:.4f}")


if __name__ == "__main__":
    main()
