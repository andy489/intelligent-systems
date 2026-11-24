# Solution — Decision Tree: Play Tennis

## Algorithm Overview

The solution implements **ID3** (Iterative Dichotomiser 3) — a top-down, greedy
decision tree induction algorithm. At each node, the attribute with the highest
**Information Gain** is selected as the split criterion. The process recurses on
each attribute-value subset until every branch reaches a pure leaf or runs out
of attributes.

---

## Entropy

For a sample set S with classes c₁, c₂, …:

```
H(S) = −Σᵢ (|Sᵢ| / |S|) · log₂(|Sᵢ| / |S|)
```

H(S) = 0 for a pure set, H(S) = 1 for a maximally impure binary set.

---

## Information Gain

```
Gain(S, A) = H(S) − Σ_{v ∈ Values(A)} (|Sᵥ| / |S|) · H(Sᵥ)
```

The attribute A with the highest gain is chosen as the split at each node.
Gain measures how much knowing A reduces uncertainty about the class label.

---

## Tree Construction (ID3)

```
build(rows, attrs, defaultClass):
  if rows empty          → Leaf(defaultClass)
  if all same class      → Leaf(that class)
  if attrs empty         → Leaf(majority(rows))

  best ← argmax_{a ∈ attrs} Gain(rows, a)
  node ← Node(best)
  for each value v of best:
      sub  ← rows where rows[best] = v
      node.children[v] ← build(sub, attrs − {best}, majority(rows))
  return node
```

**Base cases:**
- **Empty subset** — a value of `best` appears in test data but not training; fall back to parent majority.
- **Pure subset** — all same class; stop recursing immediately.
- **No attributes** — exhausted all features; take majority class.

---

## Play Tennis: Root Selection

All 14 examples: 9 Yes / 5 No → H(S) = 0.940 bits

| Attribute | Gain |
|-----------|------|
| Outlook | **0.247** ← chosen |
| Humidity | 0.151 |
| Wind | 0.048 |
| Temperature | 0.029 |

Outlook splits into:
- Sunny (5 examples, 2Y/3N) → recurse → **Humidity** wins → pure leaves
- Overcast (4 examples, 4Y/0N) → pure → **Yes** leaf
- Rain (5 examples, 3Y/2N) → recurse → **Wind** wins → pure leaves

---

## Resulting Tree

```
Outlook = Sunny
  Humidity = High   → No
  Humidity = Normal → Yes
Outlook = Overcast  → Yes
Outlook = Rain
  Wind = Weak   → Yes
  Wind = Strong → No
```

This matches the canonical result from Mitchell (1997, Chapter 3).

---

## Complexity

| Phase | Time | Space |
|-------|------|-------|
| Build tree | O(n · A · D) | O(n · D) |
| Classify one instance | O(D) | O(1) |
| Training accuracy | O(n · D) | O(1) |

Where n = instances, A = attributes, D = tree depth (≤ A).  
For Play Tennis: n=14, A=4, D=2 — trivially fast.

---

## Source Files

| Language | File |
|----------|------|
| C++ | `cpp/decision_tree.cpp` |
| Java | `java/src/DecisionTree.java` |
| Python | `python/decision_tree.py` |

---

## Build & Run

**C++**
```bash
clang++ -std=c++17 -O2 -o cpp/decision_tree cpp/decision_tree.cpp
./cpp/decision_tree data/play_tennis.csv
# Non-interactive:
./cpp/decision_tree data/play_tennis.csv "Sunny,Hot,High,Weak"
```

**Java**
```bash
javac java/src/DecisionTree.java -d java/out
java -cp java/out DecisionTree data/play_tennis.csv
# Non-interactive:
java -cp java/out DecisionTree data/play_tennis.csv "Sunny,Hot,High,Weak"
```

**Python**
```bash
python3 python/decision_tree.py data/play_tennis.csv
# Non-interactive:
python3 python/decision_tree.py data/play_tennis.csv "Sunny,Hot,High,Weak"
```

---

## Expected Results

All three implementations produce **14/14 (100.00%)** training accuracy on
`play_tennis.csv` and the identical canonical tree from the slides.
