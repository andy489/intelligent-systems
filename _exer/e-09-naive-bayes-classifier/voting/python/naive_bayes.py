import sys
import math
import random
from collections import defaultdict

import os
DATA_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), "../data/house-votes-84.data")
NUM_ATTRS = 16
VALUES = ["y", "n", "?"]
K = len(VALUES)

def value_index(v):
    if v == "y": return 0
    if v == "n": return 1
    return 2

def main():
    with open(DATA_PATH) as f:
        data = [line.strip().split(",") for line in f if line.strip()]

    random.seed(42)
    random.shuffle(data)

    n = len(data)
    num_folds = 10
    base, extra = divmod(n, num_folds)
    fold_sizes = [base + (1 if i < extra else 0) for i in range(num_folds)]

    total_acc = 0.0
    start = 0
    for fold in range(num_folds):
        end = start + fold_sizes[fold]
        test = data[start:end]
        train = data[:start] + data[end:]

        classes = list({row[0] for row in train})
        class_count = defaultdict(int)
        # counts[cls][attr][val_idx]
        counts = {cls: [[0] * K for _ in range(NUM_ATTRS)] for cls in classes}

        for row in train:
            cls = row[0]
            class_count[cls] += 1
            for a in range(NUM_ATTRS):
                vi = value_index(row[a + 1])
                counts[cls][a][vi] += 1

        total_train = len(train)
        correct = 0
        for row in test:
            true_cls = row[0]
            best_cls, best_score = None, float("-inf")
            for cls in classes:
                cc = class_count[cls]
                score = math.log(cc / total_train)
                for a in range(NUM_ATTRS):
                    vi = value_index(row[a + 1])
                    p = (counts[cls][a][vi] + 1) / (cc + K)
                    score += math.log(p)
                if score > best_score:
                    best_score, best_cls = score, cls
            if best_cls == true_cls:
                correct += 1

        acc = correct / len(test)
        total_acc += acc
        print(f"Fold {fold+1:2d}: accuracy = {acc:.4f} ({correct}/{len(test)} correct)")
        start = end

    print(f"Mean accuracy: {total_acc / num_folds:.4f}")

if __name__ == "__main__":
    main()
