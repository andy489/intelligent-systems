# k-d Tree — Nearest Neighbour Search

A k-d tree built from N 2D points, answering Q nearest-neighbour queries in O(log N) average time.

See [PROBLEM.md](PROBLEM.md) for the full problem statement and [SOLUTION.md](SOLUTION.md) for the algorithm explanation.

---

## Input Format

```
N
x1 y1
x2 y2
...
xN yN
Q
qx1 qy1
...
qxQ qyQ
```

## Example

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

Expected output:
```
Nearest to (1.00, 2.00): point 0 (2.00, 3.00) dist=1.4142
Nearest to (9.00, 3.00): point 2 (9.00, 1.00) dist=2.0000
Nearest to (5.00, 5.00): point 1 (5.00, 4.00) dist=1.0000
```

---

## Running

### Java

```bash
cd java
javac src/KdTree.java -d out
echo "5
2.0 3.0
5.0 4.0
9.0 1.0
4.0 7.0
8.0 8.0
3
1.0 2.0
9.0 3.0
5.0 5.0" | java -cp out KdTree
```

### Python

```bash
echo "5
2.0 3.0
5.0 4.0
9.0 1.0
4.0 7.0
8.0 8.0
3
1.0 2.0
9.0 3.0
5.0 5.0" | python3 python/kd_tree.py
```

### C++

```bash
g++ -std=c++17 -O2 -o kd_tree cpp/kd_tree.cpp
echo "5
2.0 3.0
5.0 4.0
9.0 1.0
4.0 7.0
8.0 8.0
3
1.0 2.0
9.0 3.0
5.0 5.0" | ./kd_tree
```

---

## File Structure

```
kd-tree/
├── PROBLEM.md          problem statement
├── SOLUTION.md         algorithm explanation & complexity analysis
├── java/
│   └── src/
│       └── KdTree.java
├── python/
│   └── kd_tree.py
└── cpp/
    └── kd_tree.cpp
```
