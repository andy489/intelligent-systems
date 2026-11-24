# Problem — Decision Tree: Play Tennis

## Statement

Implement the **ID3** (Iterative Dichotomiser 3) decision tree learning algorithm.
Given a training set of labelled examples, build a decision tree that classifies new
instances by recursively selecting the attribute with the highest **Information Gain**.

**Input:** a CSV data file and optionally a query instance.

**Output:**
- The learned tree printed with indentation showing depth
- Training accuracy
- Interactive classifier: predict the class for user-supplied attribute values

---

## Dataset

**Play Tennis** — the canonical textbook example (Mitchell, 1997).

**File:** `data/play_tennis.csv`

| Property | Value |
|----------|-------|
| Instances | 14 |
| Attributes | 4 (Outlook, Temperature, Humidity, Wind) |
| Class | PlayTennis (`Yes` / `No`) |

| Day | Outlook | Temperature | Humidity | Wind | PlayTennis |
|-----|---------|-------------|----------|------|------------|
| D1  | Sunny | Hot | High | Weak | No |
| D2  | Sunny | Hot | High | Strong | No |
| D3  | Overcast | Hot | High | Weak | Yes |
| D4  | Rain | Mild | High | Weak | Yes |
| D5  | Rain | Cool | Normal | Weak | Yes |
| D6  | Rain | Cool | Normal | Strong | No |
| D7  | Overcast | Cool | Normal | Strong | Yes |
| D8  | Sunny | Mild | High | Weak | No |
| D9  | Sunny | Cool | Normal | Weak | Yes |
| D10 | Rain | Mild | Normal | Weak | Yes |
| D11 | Sunny | Mild | Normal | Strong | Yes |
| D12 | Overcast | Mild | High | Strong | Yes |
| D13 | Overcast | Hot | Normal | Weak | Yes |
| D14 | Rain | Mild | High | Strong | No |

### Attribute values

| Attribute | Values |
|-----------|--------|
| Outlook | Sunny, Overcast, Rain |
| Temperature | Hot, Mild, Cool |
| Humidity | High, Normal |
| Wind | Weak, Strong |
| PlayTennis (class) | Yes, No |

---

## Formal Definition

### ID3 Algorithm

```
ID3(examples, attributes, default):
  if examples is empty:        return Leaf(default)
  if all examples same class:  return Leaf(that class)
  if attributes is empty:      return Leaf(majority class)

  A ← attribute with highest Gain(examples, A)
  tree ← Node(A)
  for each value v of A:
      subtree ← ID3(examples where A=v, attributes − {A}, majority(examples))
      tree.addBranch(v, subtree)
  return tree
```

### Entropy

Measures impurity of a sample set S:

```
H(S) = −Σ pᵢ log₂(pᵢ)
```

where pᵢ is the proportion of class i in S.

- H(S) = 0 → pure (all same class)
- H(S) = 1 → maximally impure (equal split, binary class)

### Information Gain

Expected reduction in entropy from splitting on attribute A:

```
Gain(S, A) = H(S) − Σ_{v ∈ Values(A)} (|Sᵥ| / |S|) · H(Sᵥ)
```

where Sᵥ is the subset of S where attribute A has value v.

**Root selection on Play Tennis (all 14 examples, 9 Yes / 5 No):**

```
H(S)                = −(9/14)log₂(9/14) − (5/14)log₂(5/14) = 0.940

Gain(S, Outlook)     = 0.940 − (5/14)·0.971 − (4/14)·0.0 − (5/14)·0.971 = 0.247
Gain(S, Temperature) = 0.940 − (4/14)·1.0   − (6/14)·0.918 − (4/14)·0.811 = 0.029
Gain(S, Humidity)    = 0.940 − (7/14)·0.985 − (7/14)·0.592 = 0.151
Gain(S, Wind)        = 0.940 − (8/14)·1.0   − (6/14)·1.0   = 0.048  (corrected: 0.048)
```

Outlook has the highest gain → becomes the root.

---

## Expected Output

```
Decision Tree:
Outlook = Sunny
  Humidity = High -> No
  Humidity = Normal -> Yes
Outlook = Overcast -> Yes
Outlook = Rain
  Wind = Weak -> Yes
  Wind = Strong -> No

Training accuracy: 14/14 (100.00%)

Enter attributes (Outlook Temperature Humidity Wind) or 'quit':
> Sunny Hot High Weak
Prediction: No
> Overcast Cool Normal Weak
Prediction: Yes
> quit
```

---

## Special Cases

| Case | Behaviour |
|------|-----------|
| All examples same class | Return leaf immediately |
| No attributes left | Return leaf with majority class |
| No examples in branch | Return leaf with parent's majority class |
| Unknown value at query time | Follow first available branch |
