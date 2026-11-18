# TSP — Travelling Salesman Problem via Genetic Algorithm

See [PROBLEM.md](PROBLEM.md) for the problem statement and
[SOLUTION.md](SOLUTION.md) for the full algorithm description.

---

## Implementations

| Language | Entry point |
|----------|-------------|
| C++    | `cpp/tsp_ga.cpp` |
| Java   | `java/src/TspGa.java` |
| Python | `python/tsp_ga.py` |

All three implement the **identical algorithm** with the same hyper-parameters
and fixed seed, so they produce the same output.

---

## Build

### C++
```bash
cd cpp
g++ -std=c++17 -O2 -o tsp_ga tsp_ga.cpp
```

### Java
```bash
cd java
mkdir -p out
javac -d out src/TspGa.java
```

### Python
No build step needed — requires Python 3.9+.

---

## Running

Every program asks two or three questions interactively.
You can also pipe the answers directly (useful for scripting):

```
<mode>   →  1 = random cities   2 = UK-12 benchmark
<N>      →  number of cities    (mode 1 only, recommended ≤ 100)
<P>      →  population size     (see guidance below)
```

### C++
```bash
cd cpp

# interactive
./tsp_ga

# piped — random 20 cities, population 300
echo "1
20
300" | ./tsp_ga

# piped — UK-12 benchmark, population 500
echo "2
500" | ./tsp_ga
```

### Java
```bash
cd java

# interactive
java -cp out TspGa

# piped — random 20 cities, population 300
echo "1
20
300" | java -cp out TspGa

# piped — UK-12 benchmark, population 500
echo "2
500" | java -cp out TspGa
```

### Python
```bash
cd python

# interactive
python3 tsp_ga.py

# piped — random 20 cities, population 300
echo "1
20
300" | python3 tsp_ga.py

# piped — UK-12 benchmark, population 500
echo "2
500" | python3 tsp_ga.py
```

---

## Choosing the population size P

Population size is the single most impactful tuning knob.

| Scenario | Recommended P | Notes |
|----------|--------------|-------|
| UK-12 benchmark | **300 – 500** | Reliably finds the optimal in < 300 generations |
| Random N = 20 | **200 – 400** | Good balance of speed and quality |
| Random N = 50 | **400 – 800** | Larger search space needs more diversity |
| Random N = 100 | **800 – 1500** | Slow per generation; use only if time permits |

**Rule of thumb:** start with `P = 20 × N`.  
If the result looks stuck in a local optimum, double P and rerun.

Larger P → more diversity → better solutions, but each generation takes longer.  
The stagnation guard (200 generations without improvement) stops the run early
when the population has converged, so very large P does not waste time indefinitely.

---

## UK-12 Benchmark

The `data/` folder contains a 12-city UK dataset with coordinates in a
custom Cartesian system (km-scale offsets, Aberystwyth as approximate origin).

**True optimal tour length: 1872.7806**
verified by the Held-Karp exact algorithm (dynamic programming, 2^N · N² steps).

```
Aberystwyth → Inverness → Brighton → Newcastle → Oxford → Liverpool →
Exeter → Stratford → London → Edinburgh → Glasgow → Nottingham
```

> **Note on the value 1595.74:** this figure appears in some academic references
> for a dataset with the same city names but **different coordinates** (typically
> an integer grid from a textbook).  It does not apply to the CSV files here.
> The images `data/cities.png` and `data/tour.png` were generated from the actual
> CSV coordinates and confirm 1872.78 is the correct benchmark for this data.

A correctly implemented GA with P ≥ 300 consistently finds this optimal.

---

## Hyper-parameters (all three implementations)

| Parameter | Value | Effect |
|-----------|-------|--------|
| `NEW_GEN_PCT` | 0.60 | 60 % of P created as offspring per generation |
| `SURVIVE_PCT` | 0.50 | 50 % of P eligible as parents (SUS selection) |
| `ELITISM_PCT` | 0.02 | 2 % of P carried unchanged (best individuals) |
| `STAGNATION_GEN` | 200 | Stop after 200 generations with no improvement |
| `STAGNATION_EPS` | 0.01 | Minimum improvement to reset stagnation counter |
| `MAX_GEN` | 2000 | Hard cap on number of generations |
| Crossover mix | OX 3/6, 2-pt 2/6, CX 1/6 | Order XO dominates; CX used sparingly |
| Mutation mix | inversion 2/6, insert 2/6, shuffle 2/6 | Equal weight for diversity |
| Immigrant injection | 10 % every 20 gens | Replaces worst individuals with random tours |
