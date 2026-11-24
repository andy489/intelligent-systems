# Problem — Naïve Bayes Classifier: Congressional Voting Records

## Statement

Implement a **Naïve Bayes Classifier** that classifies US House of Representatives members
as **democrat** or **republican** based on their votes on 16 key issues.

Evaluate the classifier using **10-fold cross-validation**: randomly shuffle the dataset,
split it into 10 equal folds, and perform 10 train/test cycles — each fold is used exactly
once as the test set while the remaining 9 folds form the training set.

Print the **accuracy** for each fold and the **mean accuracy** across all 10 folds.

---

## Dataset

**Source:** [UCI Congressional Voting Records](http://archive.ics.uci.edu/ml/datasets/Congressional+Voting+Records)  
**File:** `data/house-votes-84.data`

| Property | Value |
|----------|-------|
| Instances | 435 |
| Classes | `democrat`, `republican` |
| Attributes | 16 votes (one per issue) |
| Vote values | `y` (yes), `n` (no), `?` (abstain / missing) |

Each row:
```
class,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15,v16
```

Example rows:
```
republican,n,y,n,y,y,y,n,n,n,y,?,y,y,y,n,y
democrat,?,y,y,?,y,y,n,n,n,n,y,n,n,n,y,?
```

---

## Preprocessing

- Treat `?` as a **third valid vote value** — not as missing data.  
  Each attribute therefore has exactly 3 possible values: `y`, `n`, `?`.
- Apply **Laplace smoothing** (α = 1) to avoid zero probabilities for unseen value/class combinations.

---

## Formal Definition

**Naïve Bayes** classifies a new instance **x = (x₁, x₂, …, x₁₆)** by:

```
ĉ = argmax  log P(c) + Σᵢ log P(xᵢ | c)
     c ∈ C
```

**Class prior** (no smoothing needed with enough data):
```
P(c) = count(c) / N
```

**Conditional probability with Laplace smoothing** (K = 3 possible values per attribute):
```
P(v | c, attr_i) = (count(v, c, attr_i) + 1) / (count(c) + K)
```

Use **log probabilities** throughout to avoid floating-point underflow when
multiplying 16 small probabilities.

---

## 10-Fold Cross-Validation Procedure

1. Shuffle the 435 instances randomly (use seed = 42 for reproducibility).
2. Split into 10 folds of approximately equal size (435 ÷ 10 = 43 or 44 instances each).
3. For fold `k` (k = 1 … 10):
   - **Test set:** fold k
   - **Training set:** all other 9 folds combined
   - Train the Naïve Bayes classifier on the training set.
   - Classify every instance in the test set.
   - Compute accuracy = correct / total.
4. Print accuracy for each fold and the mean.

---

## Output Format

```
Fold  1: accuracy = 0.9318 (41/44 correct)
Fold  2: accuracy = 0.9091 (40/44 correct)
Fold  3: accuracy = 0.9545 (42/44 correct)
Fold  4: accuracy = 0.9091 (40/44 correct)
Fold  5: accuracy = 0.8864 (39/44 correct)
Fold  6: accuracy = 0.9318 (41/44 correct)
Fold  7: accuracy = 0.9545 (42/44 correct)
Fold  8: accuracy = 0.9545 (42/44 correct)
Fold  9: accuracy = 0.9318 (41/43 correct)
Fold 10: accuracy = 0.9070 (39/43 correct)
Mean accuracy: 0.9271
```

*(Exact numbers depend on shuffle; expect 90–93 % mean accuracy.)*

---

## Special Cases

| Case | Expected behaviour |
|------|--------------------|
| All training instances belong to one class | P(other class) = 0; classifier always predicts the present class |
| Empty fold | Skip fold, do not include in mean |
| Attribute value in test set not seen in training | Laplace smoothing gives `1 / (count(c) + 3)` — never zero |
