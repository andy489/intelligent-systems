# Neural Network — Boolean Functions via Backpropagation

A self-learning neural network trained with **Backpropagation** that learns the **XOR** Boolean function.

See [PROBLEM.md](PROBLEM.md) for the full problem statement and [SOLUTION.md](SOLUTION.md)
for the algorithm explanation.

---

## Network Architecture

```
Input layer     Hidden layer     Output layer
   x₁ ────┐
           ├──► h₁ ──┐
   x₂ ────┘           ├──► o₁   (sigmoid)
           ├──► h₂ ──┘
   bias ───┘    ...
```

| Parameter | Value |
|-----------|-------|
| Input neurons | 2 |
| Hidden neurons H | 4 (configurable) |
| Output neurons | 1 |
| Activation | sigmoid: `1 / (1 + e^(−x))` |
| Learning rate η | 0.9 |
| Momentum | 0.7 |
| Weight init range | (−0.05, 0.05) |
| Max epochs | 50 000 |
| Stop condition | sum of squared errors < 0.001 |

---

## Why XOR?

AND and OR are **linearly separable** — a single straight decision boundary can correctly
classify all four input combinations. Any single-layer network (or even a network with
no hidden layer) can learn them trivially.

XOR is **not linearly separable**: no straight line can separate the "true" inputs
`{(0,1), (1,0)}` from the "false" inputs `{(0,0), (1,1)}`.

```
  x₂
  1 |  0   1       ← (0,1)=1, (1,1)=0
    |
  0 |  0   1       ← (0,0)=0, (1,0)=1
    +----------
       0   1   x₁

No single line can separate the 1s from the 0s.
```

A **hidden layer with ≥ 2 neurons** solves this by composing two linear boundaries
into a non-linear decision region. This is the core insight of the exercise — and why
the XOR problem historically motivated the development of multi-layer networks and
the Backpropagation algorithm.

> With H = 1 hidden neuron the network **cannot converge** on XOR — see TESTS.md TC4.

---

## Quick Start

From the `e-12-neural-networks/` directory:

### C++

```bash
# Compile
g++ -std=c++17 -O2 -o cpp/neural_network cpp/neural_network.cpp

# Run
./cpp/neural_network
```

### Python

```bash
python3 python/neural_network.py
```

### Java

```bash
# Compile
javac java/src/Connection.java java/src/Neuron.java java/src/NeuralNetwork.java \
      -d java/out

# Run
java -cp java/out NeuralNetwork
```

---

## Customising the Network

To change the Boolean function or hyperparameters, edit the respective source file:

### C++ / Python — change function

Both iterate over AND → OR → XOR automatically. To run a single function, edit the
`functions` list at the top of `main()` / `if __name__ == "__main__"`.

### Java — change function / architecture

In `NeuralNetwork.java`:

```java
// Architecture: NeuralNetwork(inputs, hidden, outputs)
NeuralNetwork nn = new NeuralNetwork(2, 4, 1);

// Training budget
int maxRuns = 50000;
double minErrorCondition = 0.001;

// Training data (XOR shown — swap for AND/OR)
final double inputs[][]          = { {1,1}, {1,0}, {0,1}, {0,0} };
final double expectedOutputs[][] = { {0},   {1},   {1},   {0}   };
```

**AND training data:**
```java
final double inputs[][]          = { {0,0}, {0,1}, {1,0}, {1,1} };
final double expectedOutputs[][] = { {0},   {0},   {0},   {1}   };
```

**OR training data:**
```java
final double inputs[][]          = { {0,0}, {0,1}, {1,0}, {1,1} };
final double expectedOutputs[][] = { {0},   {1},   {1},   {1}   };
```

> **Note:** XOR requires at least H = 2 hidden neurons. With H = 1 the network
> cannot converge because XOR is not linearly separable.

---

## Interpreting the Output

```
Training XOR for 50000 epochs max (H=4, lr=0.9)
  INPUTS: 0 0  EXPECTED: 0  ACTUAL: 0.01225819   ← near 0 → predicts 0 ✓
  INPUTS: 0 1  EXPECTED: 1  ACTUAL: 0.98135360   ← near 1 → predicts 1 ✓
  INPUTS: 1 0  EXPECTED: 1  ACTUAL: 0.98978951   ← near 1 → predicts 1 ✓
  INPUTS: 1 1  EXPECTED: 0  ACTUAL: 0.01994020   ← near 0 → predicts 0 ✓
  Sum of squared errors = 9.99817044e-04
  EPOCH 5342
  SUCCESS: converged
```

- **ACTUAL** is the raw sigmoid output in [0, 1]. Round to 0 or 1 to get the Boolean prediction.
- **Sum of squared errors** is the total SSE across all four training patterns at the final epoch.
- **EPOCH** is the number of full passes over the training set before convergence.
- **SUCCESS** means SSE < 0.001 was reached; `!Error: failed to converge` means the budget ran out.

---

## Expected Results

Exact epoch counts vary between runs (random weight initialisation), but the pattern is consistent.

### C++

```
Training AND for 50000 epochs max (H=4, lr=0.9)
  INPUTS: 0 0  EXPECTED: 0  ACTUAL: 0.00003253
  INPUTS: 0 1  EXPECTED: 0  ACTUAL: 0.01352150
  INPUTS: 1 0  EXPECTED: 0  ACTUAL: 0.01352293
  INPUTS: 1 1  EXPECTED: 1  ACTUAL: 0.97482417
  Sum of squared errors = 9.99523898e-04
  EPOCH 1716
  SUCCESS: converged

Training OR for 50000 epochs max (H=4, lr=0.9)
  INPUTS: 0 0  EXPECTED: 0  ACTUAL: 0.02533213
  INPUTS: 0 1  EXPECTED: 1  ACTUAL: 0.98661133
  INPUTS: 1 0  EXPECTED: 1  ACTUAL: 0.98667382
  INPUTS: 1 1  EXPECTED: 1  ACTUAL: 0.99908323
  Sum of squared errors = 9.99400671e-04
  EPOCH 1426
  SUCCESS: converged

Training XOR for 50000 epochs max (H=4, lr=0.9)
  INPUTS: 0 0  EXPECTED: 0  ACTUAL: 0.01225819
  INPUTS: 0 1  EXPECTED: 1  ACTUAL: 0.98135360
  INPUTS: 1 0  EXPECTED: 1  ACTUAL: 0.98978951
  INPUTS: 1 1  EXPECTED: 0  ACTUAL: 0.01994020
  Sum of squared errors = 9.99817044e-04
  EPOCH 5342
  SUCCESS: converged
```

### Python

```
Training AND for 50000 epochs max (H=4, lr=0.9)
  INPUTS: 0 0  EXPECTED: 0  ACTUAL: 0.00003218
  INPUTS: 0 1  EXPECTED: 0  ACTUAL: 0.01354054
  INPUTS: 1 0  EXPECTED: 0  ACTUAL: 0.01349762
  INPUTS: 1 1  EXPECTED: 1  ACTUAL: 0.97482327
  Sum of squared errors = 9.99e-04
  EPOCH 1712
  SUCCESS: converged

Training OR for 50000 epochs max (H=4, lr=0.9)
  INPUTS: 0 0  EXPECTED: 0  ACTUAL: 0.02515012
  INPUTS: 0 1  EXPECTED: 1  ACTUAL: 0.98648792
  INPUTS: 1 0  EXPECTED: 1  ACTUAL: 0.98648538
  INPUTS: 1 1  EXPECTED: 1  ACTUAL: 0.99872529
  Sum of squared errors = 9.99e-04
  EPOCH 1398
  SUCCESS: converged

Training XOR for 50000 epochs max (H=4, lr=0.9)
  INPUTS: 0 0  EXPECTED: 0  ACTUAL: 0.00794308
  INPUTS: 0 1  EXPECTED: 1  ACTUAL: 0.98167613
  INPUTS: 1 0  EXPECTED: 1  ACTUAL: 0.98540829
  INPUTS: 1 1  EXPECTED: 0  ACTUAL: 0.01969925
  Sum of squared errors = 1.00e-03
  EPOCH 4099
  SUCCESS: converged
```

### Java (XOR only — hardcoded in main)

```
NN example with XOR
INPUTS: 1.0 1.0 EXPECTED: 0.0 ACTUAL: 0.00412668
INPUTS: 1.0 0.0 EXPECTED: 1.0 ACTUAL: 0.98616766
INPUTS: 0.0 1.0 EXPECTED: 1.0 ACTUAL: 0.98588914
INPUTS: 0.0 0.0 EXPECTED: 0.0 ACTUAL: 0.02434128

Sum of squared errors = 9.999770007090606E-4
##### EPOCH 10309

SUCCESS: converged
```

---

## File Structure

```
e-12-neural-networks/
├── PROBLEM.md                  problem statement
├── SOLUTION.md                 algorithm walkthrough & complexity
├── TESTS.md                    5 test cases with expected output
├── README.md                   this file
├── e-12-neural-networks-(slides).pdf
├── cpp/
│   └── neural_network.cpp      C++17 single-file implementation
├── python/
│   └── neural_network.py       Python 3 implementation
└── java/
    └── src/
        ├── Connection.java     edge between neurons
        ├── Neuron.java         node with sigmoid activation
        └── NeuralNetwork.java  training loop & main
```
