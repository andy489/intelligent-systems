# Solution — Naïve Bayes Classifier: Congressional Voting Records

## Algorithm Overview

The solution uses a **Naïve Bayes Classifier** — a probabilistic model that applies
Bayes' theorem with the "naïve" assumption that all attributes are conditionally independent
given the class label.

Despite this simplifying assumption, Naïve Bayes performs surprisingly well on real-world
categorical data and is particularly robust to the small training sets that arise in
cross-validation.

---

## Preprocessing

Each of the 16 votes takes one of three values: `y`, `n`, or `?`.  
The `?` value (abstain/missing) is treated as a **third legitimate category**, not
as missing data. This avoids the complexity of imputation and works well in practice
because abstention is itself politically informative.

---

## Model: Conditional Probabilities with Laplace Smoothing

For each class `c ∈ {democrat, republican}` and each attribute `i` (0–15):

**Count table:** `count[c][i][v]` = number of training instances of class `c`
where attribute `i` has value `v`.

**Smoothed conditional probability** (K = 3 possible values):
```
P(v | c, i) = (count[c][i][v] + 1) / (count[c] + K)
            = (count[c][i][v] + 1) / (count[c] + 3)
```

Where `count[c]` is the total number of training instances of class `c`.

> **Why K = 3?** Laplace smoothing adds 1 to every possible value. With K = 3 values
> per attribute, the denominator adds K = 3 to preserve the proper normalisation:
> Σ_v P(v|c,i) = 1.

**Class prior** (no smoothing needed with 435 instances):
```
P(c) = count[c] / N
```

---

## Classification

For a test instance **x = (x₁, …, x₁₆)**, predict the class with the highest
**log posterior**:

```
ĉ = argmax  log P(c) + Σᵢ₌₀¹⁵ log P(xᵢ | c, i)
     c
```

Using **log probabilities** avoids floating-point underflow that would occur when
multiplying 16 probabilities each less than 1.

---

## 10-Fold Cross-Validation

```
shuffle(data, seed=42)
folds ← split data into 10 equal parts (last 2 folds may have 43 instead of 44)

for k = 1 to 10:
    test  ← folds[k]
    train ← all folds except folds[k]

    build count tables from train
    for each instance in test:
        predict class with highest log-posterior
        compare to true label
    accuracy[k] ← correct / |test|
    print fold result

print mean(accuracy)
```

The 435 instances divide into 8 folds of 44 and 2 folds of 43 (8×44 + 2×43 = 352 + 86 = 438 ≠ 435).
Actually: 435 = 10 × 43 + 5, so 5 folds have 44 instances and 5 folds have 43 instances.

---

## Complexity

| Phase | Time | Space |
|-------|------|-------|
| Build count tables | O(N × A) per fold | O(C × A × K) |
| Classify one instance | O(C × A) | O(1) |
| Full 10-fold CV | O(10 × N × A) | O(C × A × K) |

Where N = 435, A = 16 attributes, C = 2 classes, K = 3 values.
Total work is effectively O(N × A) — extremely fast.

---

## Expected Results

On this dataset with seed = 42, expect **90–93 % mean accuracy**.
The classifier benefits from the fact that voting patterns are highly correlated with
party membership — many votes are nearly perfect predictors.

---

## Source Files

| Language | File |
|----------|------|
| Java | `java/src/NaiveBayes.java` |
| Python | `python/naive_bayes.py` |
| C++ | `cpp/naive_bayes.cpp` |

All three implement the identical algorithm with the same seed and parameters.

---

## Build & Run

**Java**
```bash
cd voting/java
javac src/NaiveBayes.java -d out
java -cp out NaiveBayes
```

**Python**
```bash
python3 voting/python/naive_bayes.py
```

**C++**
```bash
g++ -std=c++17 -O2 -o naive_bayes voting/cpp/naive_bayes.cpp
cd voting/cpp && ./naive_bayes
```

*(Each implementation reads `../data/house-votes-84.data` relative to its own directory.)*
