# Problem — Neural Network: Boolean Functions via Backpropagation

## Statement

Implement a **self-learning Neural Network** trained with the **Backpropagation** algorithm.
The network must learn three Boolean functions:

| Function | Linearly separable? | Architecture |
|----------|--------------------|-----------------------------|
| AND      | Yes                | 2 inputs → hidden layer → 1 output |
| OR       | Yes                | 2 inputs → hidden layer → 1 output |
| XOR      | **No**             | 2 inputs → hidden layer → 1 output |

> XOR is not linearly separable and **requires at least one hidden layer** with ≥ 2 neurons.

**Input:** the Boolean function to solve (AND / OR / XOR), number of hidden neurons, learning
rate η, and number of training epochs K.

**Output:** predicted output for all four input combinations, and whether the network
learned the function correctly.

---

## Truth Tables (Training Data)

### AND

| x₁ | x₂ | target |
|----|----|--------|
| 0  | 0  |   0    |
| 0  | 1  |   0    |
| 1  | 0  |   0    |
| 1  | 1  |   1    |

### OR

| x₁ | x₂ | target |
|----|----|--------|
| 0  | 0  |   0    |
| 0  | 1  |   1    |
| 1  | 0  |   1    |
| 1  | 1  |   1    |

### XOR

| x₁ | x₂ | target |
|----|----|--------|
| 0  | 0  |   0    |
| 0  | 1  |   1    |
| 1  | 0  |   1    |
| 1  | 1  |   0    |

---

## Architecture

```
Input layer     Hidden layer     Output layer
   x₁ ──────┐
             ├──► h₁ ──┐
   x₂ ──────┘           ├──► o₁  (sigmoid)
             ├──► h₂ ──┘
   bias ─────┘
```

- **Input nodes:** 2 (one per Boolean variable) + 1 bias node (always 1)
- **Hidden nodes:** configurable parameter H (bias included)
- **Output nodes:** 1
- **Activation:** sigmoid σ(x) = 1 / (1 + e^(−x)) at every non-input node

---

## Formal Definition

### Forward Pass

For each node i (hidden or output), compute:

```
sum_i  = Σⱼ  w_ji · x_j        (weighted sum of inputs including bias)
o_i    = σ(sum_i) = 1 / (1 + e^(−sum_i))
```

### Backpropagation — Error Terms

**Output node k:**
```
δ_k = o_k · (1 − o_k) · (t_k − o_k)
```
where t_k is the target value and o_k is the actual output.

**Hidden node h:**
```
δ_h = o_h · (1 − o_h) · Σ_{k ∈ outputs} w_kh · δ_k
```

### Weight Update

For every weight w_ij in the network:
```
w_ij ← w_ij + η · δ_j · x_i
```
where η is the learning rate, δ_j is the error at node j, and x_i is the activation
of the node at the start of the edge.

---

## Algorithm

```
1. Initialise all weights uniformly at random from (−0.05, 0.05)
2. Repeat K epochs:
   a. For each training example (x₁, x₂, target):
      i.  Forward pass: compute activations of hidden and output layers
      ii. Backprop:    compute δ for every output and hidden node
      iii.Update:      adjust every weight by η · δ · activation
3. After training, run all four input combinations and round output to 0 or 1
```

---

## Parameters

| Parameter | Meaning | Suggested value |
|-----------|---------|----------------|
| H | number of hidden neurons | 2 (minimum for XOR) |
| K | training epochs | 10 000 |
| η (eta) | learning rate | 0.1 |
| init range | weight initialisation | (−0.05, 0.05) |

---

## Expected Output

```
Training AND  for 10000 epochs  (H=2, η=0.1)
  [0,0] → 0.00003253  (expected 0)
  [0,1] → 0.01352150  (expected 0)
  [1,0] → 0.01352293  (expected 0)
  [1,1] → 0.97482417  (expected 1)
  Result: LEARNED ✓

Training OR   for 10000 epochs  (H=2, η=0.1)
  [0,0] → 0.02533213  (expected 0)
  [0,1] → 0.98661133  (expected 1)
  [1,0] → 0.98667382  (expected 1)
  [1,1] → 0.99908323  (expected 1)
  Result: LEARNED ✓

Training XOR  for 10000 epochs  (H=2, η=0.1)
  [0,0] → 0.01225819  (expected 0)
  [0,1] → 0.98135360  (expected 1)
  [1,0] → 0.98978951  (expected 1)
  [1,1] → 0.01994020  (expected 0)
  Result: LEARNED ✓
```

A prediction is considered correct when the rounded output matches the target.
The network has "learned" the function when all four predictions are correct.

---

## Special Cases

| Case | Behaviour |
|------|-----------|
| XOR with H = 1 | Will never converge — use H ≥ 2 |
| η too large (> 0.9) | Weights oscillate; network may not converge |
| η too small (< 0.001) | Convergence is very slow |
| K too small | Network may not finish learning; increase epochs |
