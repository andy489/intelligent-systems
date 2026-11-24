# Naïve Bayes Classifier — Congressional Voting Records

Classifies US House members as **democrat** or **republican** based on 16 votes,
evaluated with **10-fold cross-validation**.

See [PROBLEM.md](PROBLEM.md) for the full problem statement and [SOLUTION.md](SOLUTION.md)
for the algorithm explanation.

---

## Expected Output

Exact per-fold numbers differ by language (each RNG produces a different shuffle for seed 42),
but mean accuracy is consistently **~90%** on this dataset.

**Java** (mean 89.9%)
```
Fold  1: accuracy = 0.8182 (36/44 correct)
Fold  2: accuracy = 0.9545 (42/44 correct)
Fold  3: accuracy = 0.8182 (36/44 correct)
Fold  4: accuracy = 0.9545 (42/44 correct)
Fold  5: accuracy = 0.8864 (39/44 correct)
Fold  6: accuracy = 0.8605 (37/43 correct)
Fold  7: accuracy = 0.9535 (41/43 correct)
Fold  8: accuracy = 0.9070 (39/43 correct)
Fold  9: accuracy = 0.9070 (39/43 correct)
Fold 10: accuracy = 0.9302 (40/43 correct)
Mean accuracy: 0.8990
```

**Python** (mean 90.4%)
```
Fold  1: accuracy = 0.8636 (38/44 correct)
Fold  2: accuracy = 0.8864 (39/44 correct)
Fold  3: accuracy = 0.8409 (37/44 correct)
Fold  4: accuracy = 0.8864 (39/44 correct)
Fold  5: accuracy = 1.0000 (44/44 correct)
Fold  6: accuracy = 0.9070 (39/43 correct)
Fold  7: accuracy = 0.9302 (40/43 correct)
Fold  8: accuracy = 0.9070 (39/43 correct)
Fold  9: accuracy = 0.8837 (38/43 correct)
Fold 10: accuracy = 0.9302 (40/43 correct)
Mean accuracy: 0.9035
```

**C++** (mean 90.3%)
```
Fold  1: accuracy = 0.8636 (38/44 correct)
Fold  2: accuracy = 0.9091 (40/44 correct)
Fold  3: accuracy = 0.9318 (41/44 correct)
Fold  4: accuracy = 0.8864 (39/44 correct)
Fold  5: accuracy = 0.9318 (41/44 correct)
Fold  6: accuracy = 0.8837 (38/43 correct)
Fold  7: accuracy = 0.8837 (38/43 correct)
Fold  8: accuracy = 0.9302 (40/43 correct)
Fold  9: accuracy = 0.9302 (40/43 correct)
Fold 10: accuracy = 0.8837 (38/43 correct)
Mean accuracy: 0.9034
```

---

## Running

All three can be run from any directory — the data path is resolved relative to the binary/script location.

### Python

```bash
python3 voting/python/naive_bayes.py
```

### Java

```bash
javac voting/java/src/NaiveBayes.java -d voting/java/out
java -cp voting/java/out NaiveBayes
```

### C++

```bash
g++ -std=c++17 -O2 -o voting/cpp/naive_bayes voting/cpp/naive_bayes.cpp
voting/cpp/naive_bayes
```

---

## File Structure

```
voting/
├── PROBLEM.md              problem statement
├── SOLUTION.md             algorithm explanation & complexity
├── data/
│   └── house-votes-84.data UCI dataset (435 instances, 16 votes + class)
├── java/
│   └── src/
│       └── NaiveBayes.java
├── python/
│   └── naive_bayes.py
└── cpp/
    └── naive_bayes.cpp
```
