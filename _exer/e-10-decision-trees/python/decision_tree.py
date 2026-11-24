import sys
import os
import csv
import math
from collections import Counter

# ─── Data ─────────────────────────────────────────────────────────────────────

def read_csv(path):
    with open(path, newline="") as f:
        reader = csv.reader(f)
        rows = [r for r in reader if r]
    headers = rows[0]
    data    = rows[1:]
    return headers, data

# ─── Entropy / Gain ───────────────────────────────────────────────────────────

def entropy(rows, class_col):
    cnt = Counter(r[class_col] for r in rows)
    n = len(rows)
    return -sum((c / n) * math.log2(c / n) for c in cnt.values())

def gain(rows, attr, class_col):
    base = entropy(rows, class_col)
    parts = {}
    for r in rows:
        parts.setdefault(r[attr], []).append(r)
    n = len(rows)
    rem = sum((len(sub) / n) * entropy(sub, class_col) for sub in parts.values())
    return base - rem

# ─── ID3 ──────────────────────────────────────────────────────────────────────

def majority(rows, class_col):
    return Counter(r[class_col] for r in rows).most_common(1)[0][0]

def build(rows, attrs, class_col, default):
    if not rows:
        return ("leaf", default)
    classes = [r[class_col] for r in rows]
    if len(set(classes)) == 1:
        return ("leaf", classes[0])
    if not attrs:
        return ("leaf", majority(rows, class_col))

    best = max(attrs, key=lambda a: gain(rows, a, class_col))
    maj  = majority(rows, class_col)

    parts = {}
    for r in rows:
        parts.setdefault(r[best], []).append(r)

    remaining = [a for a in attrs if a != best]
    children  = {v: build(sub, remaining, class_col, maj)
                 for v, sub in parts.items()}
    return ("node", best, children)

# ─── Print tree ───────────────────────────────────────────────────────────────

def print_tree(node, headers, prefix=""):
    if node[0] == "leaf":
        return
    _, attr, children = node
    for val, child in children.items():
        if child[0] == "leaf":
            print(f"{prefix}{headers[attr]} = {val} -> {child[1]}")
        else:
            print(f"{prefix}{headers[attr]} = {val}")
            print_tree(child, headers, prefix + "  ")

# ─── Classify ─────────────────────────────────────────────────────────────────

def classify(node, row):
    if node[0] == "leaf":
        return node[1]
    _, attr, children = node
    child = children.get(row[attr])
    if child is None:
        return next(iter(children.values()))[1]
    return classify(child, row)

# ─── Main ─────────────────────────────────────────────────────────────────────

def main():
    csv_path = sys.argv[1] if len(sys.argv) >= 2 else \
        os.path.join(os.path.dirname(os.path.abspath(__file__)), "../data/play_tennis.csv")
    query = sys.argv[2] if len(sys.argv) >= 3 else ""

    headers, rows = read_csv(csv_path)
    class_col = len(headers) - 1
    attrs = list(range(class_col))

    root = build(rows, attrs, class_col, "")

    if query:
        print(classify(root, query.split(",")))
        return

    print("Decision Tree:")
    print_tree(root, headers)

    correct = sum(1 for r in rows if classify(root, r) == r[class_col])
    n = len(rows)
    print(f"\nTraining accuracy: {correct}/{n} ({100 * correct / n:.2f}%)")

    attr_list = " ".join(headers[:class_col])
    print(f"\nEnter attributes ({attr_list}) or 'quit':")
    while True:
        try:
            line = input("> ").strip()
        except EOFError:
            break
        if line == "quit":
            break
        vals = line.split()
        if len(vals) != class_col:
            print(f"Expected {class_col} values.")
            continue
        print(f"Prediction: {classify(root, vals)}")

if __name__ == "__main__":
    main()
