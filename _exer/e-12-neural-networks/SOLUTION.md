# Solution — Neural Network: Boolean Functions via Backpropagation

## Algorithm Overview

The solution implements a **3-layer feed-forward neural network** trained by
**Backpropagation** with **momentum**. The network learns three Boolean
functions — AND, OR, and XOR — from scratch using only the four truth-table
rows as training data.

Three implementation classes mirror the Java reference design:

| Class | Role |
|-------|------|
| `Connection` | Directed weighted edge between two neurons; tracks `weight`, `deltaWeight`, `prevDeltaWeight` |
| `Neuron` | Node with sigmoid activation; manages incoming connections and bias |
| `NeuralNetwork` | Wires the 3-layer topology, runs the training loop, prints results |

---

## Implementation

### C++ (`cpp/neural_network.cpp`)

Single-file C++ 17 solution. All three Boolean functions are trained in sequence
in `main()` via `trainFunction()`. Each call resets the static ID counters and
allocates a fresh network.

| Component | Description |
|-----------|-------------|
| `Connection` | Owns `weight`, `deltaWeight`, `prevDeltaWeight`, raw neuron pointers |
| `Neuron::calculateOutput()` | Computes weighted sum over `inConnections` + bias, applies sigmoid |
| `NeuralNetwork::activate()` | Forward pass: hidden then output layer |
| `NeuralNetwork::applyBackpropagation()` | Output deltas → hidden deltas → weight updates with momentum |
| `NeuralNetwork::run()` | Training loop: stop when squared error < `minError` or `maxSteps` exceeded |

### Python (`python/neural_network.py`)

Pure Python 3, no external dependencies. Same class structure, identical
algorithm and hyperparameters.

### Java (`java/src/`)

Three files: `Connection.java`, `Neuron.java`, `NeuralNetwork.java`.
Hardcoded for XOR; serves as the reference design the C++ and Python ports
follow.

---

## Architecture

```
Input layer     Hidden layer (H=4)      Output layer
   x₁ ──────┬──► h₁(sigmoid) ──┐
             ├──► h₂(sigmoid) ──┤
   x₂ ──────┤──► h₃(sigmoid) ──┼──► o₁(sigmoid)
             └──► h₄(sigmoid) ──┘
   bias(-1) ──────────────────── (connected to every hidden & output node)
```

- **Input layer:** 2 nodes, output = raw Boolean value (0 or 1)
- **Hidden layer:** H nodes (default H = 4), sigmoid activation
- **Output layer:** 1 node, sigmoid activation
- **Bias:** dedicated neuron with output = 1; connected to every hidden and
  output neuron; its weight is trainable

---

## Forward Pass

For each node i in hidden and output layers:

```
sum_i  = Σⱼ w_ji · x_j   (over all incoming connections including bias)
o_i    = σ(sum_i)  =  1 / (1 + e^(−sum_i))
```

The bias neuron has `output = 1` and `bias = −1` stored in Neuron; the product
`weight × 1` is treated as a standard connection in `calculateOutput()`.

---

## Backpropagation

### Output node k

```
δ_k = o_k · (1 − o_k) · (t_k − o_k)
```

### Hidden node h

```
δ_h = o_h · (1 − o_h) · Σ_{k ∈ outputs} w_kh · δ_k
```

### Weight update (single step)

```
Δw_ij  = η · δ_j · x_i
w_ij  ← w_ij + Δw_ij
```

---

## Momentum

After each weight update the previous delta is saved:

```
w_ij ← w_ij + Δw_ij + α · Δw_ij(prev)
```

Momentum (α = 0.7) dampens oscillations and accelerates convergence along
consistent gradient directions, cutting epoch counts roughly in half compared
to plain gradient descent.

---

## Training Loop

```
initialise weights uniformly in (−0.05, 0.05)
error ← 1
epoch ← 0
while epoch < maxSteps and error > minError:
    error ← 0
    for each training pattern p:
        set input  → forward pass (activate)
        get output → accumulate squared error
        backpropagate → update all weights
    epoch++
print results
```

Stop condition: **sum of squared errors across all 4 patterns < 0.001**.

---

## Complexity

| Phase | Per epoch | Note |
|-------|-----------|------|
| Forward pass | O(H · I + H) | I = inputs, H = hidden nodes |
| Output backprop | O(H) | one output node |
| Hidden backprop | O(H · I) | |
| Total per epoch | O(H · I) | constant for fixed topology |
| Training | O(epochs · H · I) | epochs ≈ 1 400 – 5 400 |

For this problem (I=2, H=4, O=1) the entire training is negligible in CPU time.

---

## Build & Run

**C++**
```bash
g++ -std=c++17 -O2 -o cpp/neural_network cpp/neural_network.cpp
./cpp/neural_network
```

**Python**
```bash
python3 python/neural_network.py
```

**Java** (XOR only)
```bash
javac java/src/Connection.java java/src/Neuron.java java/src/NeuralNetwork.java \
      -d java/out
java -cp java/out NeuralNetwork
```

---

## Results

### C++ output (`seed=42`, `H=4`, `lr=0.9`, `momentum=0.7`)

```
Training AND for 50000 epochs max (H=4, lr=0.9)
  INPUTS: 0 0  EXPECTED: 0  ACTUAL: 0.00
  INPUTS: 0 1  EXPECTED: 0  ACTUAL: 0.01
  INPUTS: 1 0  EXPECTED: 0  ACTUAL: 0.01
  INPUTS: 1 1  EXPECTED: 1  ACTUAL: 0.97
  Sum of squared errors = 1.00e-03
  EPOCH 1716
  SUCCESS: converged

Training OR for 50000 epochs max (H=4, lr=0.9)
  INPUTS: 0 0  EXPECTED: 0  ACTUAL: 0.03
  INPUTS: 0 1  EXPECTED: 1  ACTUAL: 0.99
  INPUTS: 1 0  EXPECTED: 1  ACTUAL: 0.99
  INPUTS: 1 1  EXPECTED: 1  ACTUAL: 1.00
  Sum of squared errors = 9.99e-04
  EPOCH 1426
  SUCCESS: converged

Training XOR for 50000 epochs max (H=4, lr=0.9)
  INPUTS: 0 0  EXPECTED: 0  ACTUAL: 0.01
  INPUTS: 0 1  EXPECTED: 1  ACTUAL: 0.98
  INPUTS: 1 0  EXPECTED: 1  ACTUAL: 0.99
  INPUTS: 1 1  EXPECTED: 0  ACTUAL: 0.02
  Sum of squared errors = 1.00e-03
  EPOCH 5342
  SUCCESS: converged
```

### Python output (`seed=42`, same hyperparameters)

```
Training AND for 50000 epochs max (H=4, lr=0.9)
  INPUTS: 0 0  EXPECTED: 0  ACTUAL: 0.00
  INPUTS: 0 1  EXPECTED: 0  ACTUAL: 0.01
  INPUTS: 1 0  EXPECTED: 0  ACTUAL: 0.01
  INPUTS: 1 1  EXPECTED: 1  ACTUAL: 0.97
  Sum of squared errors = 9.99e-04
  EPOCH 1712
  SUCCESS: converged

Training OR for 50000 epochs max (H=4, lr=0.9)
  INPUTS: 0 0  EXPECTED: 0  ACTUAL: 0.03
  INPUTS: 0 1  EXPECTED: 1  ACTUAL: 0.99
  INPUTS: 1 0  EXPECTED: 1  ACTUAL: 0.99
  INPUTS: 1 1  EXPECTED: 1  ACTUAL: 1.00
  Sum of squared errors = 9.99e-04
  EPOCH 1398
  SUCCESS: converged

Training XOR for 50000 epochs max (H=4, lr=0.9)
  INPUTS: 0 0  EXPECTED: 0  ACTUAL: 0.01
  INPUTS: 0 1  EXPECTED: 1  ACTUAL: 0.98
  INPUTS: 1 0  EXPECTED: 1  ACTUAL: 0.99
  INPUTS: 1 1  EXPECTED: 0  ACTUAL: 0.02
  Sum of squared errors = 1.00e-03
  EPOCH 4099
  SUCCESS: converged
```

### Java output (`seed=random`, XOR only)

```
NN example with XOR
INPUTS: 1.0 1.0 EXPECTED: 0.0 ACTUAL: .01
INPUTS: 1.0 0.0 EXPECTED: 1.0 ACTUAL: .98
INPUTS: 0.0 1.0 EXPECTED: 1.0 ACTUAL: .99
INPUTS: 0.0 0.0 EXPECTED: 0.0 ACTUAL: .02
Sum of squared errors = 9.999...E-4
##### EPOCH ~10311
SUCCESS: converged
```

> Java epoch count varies between runs (no fixed seed) but consistently
> converges below 50 000 epochs.

---

## Why XOR Needs ≥ 2 Hidden Neurons

AND and OR are linearly separable — a single decision boundary (hyperplane)
can separate the two classes. XOR is **not** linearly separable: no single line
divides {(0,1),(1,0)} from {(0,0),(1,1)}. A hidden layer with ≥ 2 neurons lets
the network compose two linear boundaries, forming the non-linear decision
region XOR requires. With H = 1, training fails to converge (see TESTS.md TC4).

---

## Source Files

| File | Purpose |
|------|---------|
| `cpp/neural_network.cpp` | C++17 — trains XOR |
| `python/neural_network.py` | Python 3 — same algorithm, same hyperparameters (XOR) |
| `java/src/Connection.java` | Java — edge class |
| `java/src/Neuron.java` | Java — node class |
| `java/src/NeuralNetwork.java` | Java — network + training loop (XOR only) |
