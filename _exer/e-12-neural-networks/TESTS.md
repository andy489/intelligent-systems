# Tests — Neural Network: Boolean Functions via Backpropagation

All three implementations (C++, Python, Java) can be run with the commands
below. TC1–TC3 show real output captured from C++ and Python. TC4–TC5 require
small code modifications; expected behaviour is described.

```bash
# C++ — build once
g++ -std=c++17 -O2 -o cpp/neural_network cpp/neural_network.cpp

# C++ — run all three functions
./cpp/neural_network

# Python — run all three functions
python3 python/neural_network.py

# Java — XOR only
javac java/src/Connection.java java/src/Neuron.java java/src/NeuralNetwork.java -d java/out
java -cp java/out NeuralNetwork
```

---

## TC1 — AND function

**What it tests:** Network learns the linearly-separable AND gate.
Only the input (1,1) maps to 1; all others map to 0.

**Parameters:** H=4, lr=0.9, momentum=0.7, maxEpochs=50 000, minError=0.001

**Expected output (C++):**
```
Training AND for 50000 epochs max (H=4, lr=0.9)
  INPUTS: 0 0  EXPECTED: 0  ACTUAL: 0.00
  INPUTS: 0 1  EXPECTED: 0  ACTUAL: 0.01
  INPUTS: 1 0  EXPECTED: 0  ACTUAL: 0.01
  INPUTS: 1 1  EXPECTED: 1  ACTUAL: 0.97
  Sum of squared errors = 1.00e-03
  EPOCH 1716
  SUCCESS: converged
```

**Expected output (Python):**
```
Training AND for 50000 epochs max (H=4, lr=0.9)
  INPUTS: 0 0  EXPECTED: 0  ACTUAL: 0.00
  INPUTS: 0 1  EXPECTED: 0  ACTUAL: 0.01
  INPUTS: 1 0  EXPECTED: 0  ACTUAL: 0.01
  INPUTS: 1 1  EXPECTED: 1  ACTUAL: 0.97
  Sum of squared errors = 9.99e-04
  EPOCH 1712
  SUCCESS: converged
```

---

## TC2 — OR function

**What it tests:** Network learns the linearly-separable OR gate.
Only (0,0) maps to 0; all others map to 1.

**Parameters:** same as TC1

**Expected output (C++):**
```
Training OR for 50000 epochs max (H=4, lr=0.9)
  INPUTS: 0 0  EXPECTED: 0  ACTUAL: 0.03
  INPUTS: 0 1  EXPECTED: 1  ACTUAL: 0.99
  INPUTS: 1 0  EXPECTED: 1  ACTUAL: 0.99
  INPUTS: 1 1  EXPECTED: 1  ACTUAL: 1.00
  Sum of squared errors = 9.99e-04
  EPOCH 1426
  SUCCESS: converged
```

**Expected output (Python):**
```
Training OR for 50000 epochs max (H=4, lr=0.9)
  INPUTS: 0 0  EXPECTED: 0  ACTUAL: 0.03
  INPUTS: 0 1  EXPECTED: 1  ACTUAL: 0.99
  INPUTS: 1 0  EXPECTED: 1  ACTUAL: 0.99
  INPUTS: 1 1  EXPECTED: 1  ACTUAL: 1.00
  Sum of squared errors = 9.99e-04
  EPOCH 1398
  SUCCESS: converged
```

---

## TC3 — XOR function

**What it tests:** Network learns the non-linearly-separable XOR gate.
Requires at least 2 hidden neurons. With H=4 the network converges in
~4 000–5 000 epochs.

**Parameters:** same as TC1

**Expected output (C++):**
```
Training XOR for 50000 epochs max (H=4, lr=0.9)
  INPUTS: 0 0  EXPECTED: 0  ACTUAL: 0.01
  INPUTS: 0 1  EXPECTED: 1  ACTUAL: 0.98
  INPUTS: 1 0  EXPECTED: 1  ACTUAL: 0.99
  INPUTS: 1 1  EXPECTED: 0  ACTUAL: 0.02
  Sum of squared errors = 1.00e-03
  EPOCH 5342
  SUCCESS: converged
```

**Expected output (Python):**
```
Training XOR for 50000 epochs max (H=4, lr=0.9)
  INPUTS: 0 0  EXPECTED: 0  ACTUAL: 0.01
  INPUTS: 0 1  EXPECTED: 1  ACTUAL: 0.98
  INPUTS: 1 0  EXPECTED: 1  ACTUAL: 0.99
  INPUTS: 1 1  EXPECTED: 0  ACTUAL: 0.02
  Sum of squared errors = 1.00e-03
  EPOCH 4099
  SUCCESS: converged
```

> XOR takes 3–4× more epochs than AND/OR because it requires composing two
> non-linear decision boundaries — the hidden layer must first learn internal
> representations before the output layer can combine them correctly.

---

## TC4 — XOR with H = 1 (expected failure)

**What it tests:** A single hidden neuron cannot represent XOR. The function is
not linearly separable; one hidden unit can only create one decision boundary,
which is insufficient. Training must fail to converge.

**How to run (Python):**
```python
from python.neural_network import train_function
train_function("XOR (H=1)", [[0,0],[0,1],[1,0],[1,1]], [[0],[1],[1],[0]],
               H=1, max_runs=50000, min_error=0.001)
```

**Actual output (Python, seed=42):**
```
Training XOR (H=1) for 50000 epochs max (H=1, lr=0.9)
  INPUTS: 0 0  EXPECTED: 0  ACTUAL: 0.32
  INPUTS: 0 1  EXPECTED: 1  ACTUAL: 0.30
  INPUTS: 1 0  EXPECTED: 1  ACTUAL: 1.00
  INPUTS: 1 1  EXPECTED: 0  ACTUAL: 0.34
  Sum of squared errors = 7.10e-01
  EPOCH 50000
  !Error: failed to converge
```

**Expected behaviour:** Outputs are trapped near 0.3–0.5 regardless of epoch
count. The error plateaus well above `minError`. Increasing epochs does not
help — the architecture is fundamentally incapable of solving XOR.

---

## TC5 — XOR with excessively high learning rate (lr = 50.0)

**What it tests:** A very high learning rate causes weight updates to overshoot,
pushing sigmoid outputs to saturation (≈ 0 or 1) and vanishing gradients.
The network gets stuck.

**How to run (Python):**
```python
from python.neural_network import train_function
train_function("XOR (lr=50.0)", [[0,0],[0,1],[1,0],[1,1]], [[0],[1],[1],[0]],
               H=4, lr=50.0, max_runs=50000, min_error=0.001)
```

**Actual output (Python, seed=0):**
```
Training XOR (lr=50.0) for 50000 epochs max (H=4, lr=50.0)
  INPUTS: 0 0  EXPECTED: 0  ACTUAL: 0.00
  INPUTS: 0 1  EXPECTED: 1  ACTUAL: 0.00
  INPUTS: 1 0  EXPECTED: 1  ACTUAL: 0.00
  INPUTS: 1 1  EXPECTED: 0  ACTUAL: 0.00
  Sum of squared errors = 2.00e+00
  EPOCH 50000
  !Error: failed to converge
```

**Expected behaviour:** All outputs collapse to ≈ 0.0, error ≈ 2.0 (maximum
possible for 4 patterns with output error of 0 vs 1). The weights explode on
the first few updates and all neurons saturate. The network never recovers.

> **Safe range for learning rate:** 0.01 – 1.0 for this topology with
> momentum = 0.7. Values above ~2.0 risk divergence depending on the random
> weight initialisation.

---

## Summary

| TC | Function | H | lr | Expected outcome |
|----|----------|---|----|-----------------|
| 1 | AND | 4 | 0.9 | Converges ~1 400–1 800 epochs |
| 2 | OR  | 4 | 0.9 | Converges ~1 400–1 800 epochs |
| 3 | XOR | 4 | 0.9 | Converges ~4 000–5 400 epochs |
| 4 | XOR | 1 | 0.9 | **Fails** — not linearly separable |
| 5 | XOR | 4 | 50.0 | **Fails** — weight explosion / saturation |
