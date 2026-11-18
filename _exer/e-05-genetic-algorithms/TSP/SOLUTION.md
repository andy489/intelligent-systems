# Solution — Travelling Salesman Problem via Genetic Algorithm

## Algorithm Overview

The solution uses a **Genetic Algorithm (GA)** — a population-based metaheuristic
that mimics biological evolution.  A pool of candidate tours is repeatedly selected,
crossed, and mutated until the shortest closed tour converges.

---

## Representation

Each **individual (chromosome)** is a permutation of city indices:

```
[c₀, c₁, c₂, …, c_{N−1}]
```

The tour visits cities in that order and returns to `c₀`, forming a closed loop.  
There is no start/end city — any rotation of a permutation represents the same tour.

---

## Fitness Function

The fitness of a chromosome is its **total closed-tour length**:

```
f(tour) = dist(c₀, c₁) + dist(c₁, c₂) + … + dist(c_{N−2}, c_{N−1}) + dist(c_{N−1}, c₀)
```

where `dist(a, b)` is the Euclidean distance between cities `a` and `b`.  
**Lower is better** — the GA minimises this value.

All N² pairwise distances are precomputed once in O(N²) and stored in a matrix
so every fitness evaluation is O(N) instead of O(N²).

> **Bug in naive implementations:** omitting the closing edge `c_{N-1} → c₀`
> silently underestimates every tour, corrupting selection pressure.

---

## Initialisation

1. Randomly generate N city coordinates in [0, 10 000]².
2. Create P individuals by randomly shuffling the city index array `[0, 1, …, N−1]`.
3. Evaluate fitness for every individual.
4. Sort the population **ascending** by tour length — `pop[0]` is always the best individual.

---

## Selection — Stochastic Universal Sampling (SUS)

**Stochastic Universal Sampling** selects `⌊P × SURVIVE_PCT⌋` parent indices using
a single random starting point and evenly spaced pointers across the fitness distribution.
This gives every individual a fair, low-variance chance proportional to its fitness.

Because fitness is a *distance* (lower = better), the raw values are **inverted** before
building the cumulative distribution:

```
inv[i] = maxFit − fit[i] + 1
```

This maps the shortest tour to the largest weight (highest selection probability) and
the longest tour to the smallest weight (weight ≥ 1, never zero).

```
cumulative[i] = Σ_{j=0}^{i} inv[j] / totalInv

step  = 1 / sampleSize
start = Uniform(0, step)

for k in 0 .. sampleSize-1:
    advance j until cumulative[j] ≥ start + k × step
    selected[k] = j
```

> **Bug in naive implementations:** using raw fitness (distance) in the numerator
> inverts selection pressure — longer tours are selected *more* often.

---

## Crossover

Three crossover operators are chosen uniformly at random per pair of parents.  
All operators produce **valid permutations** (no duplicate or missing cities).

### Order Crossover (OX) — primary operator

1. Choose a random segment `[l, r]` from the **primary** parent and copy it verbatim.
2. Fill the remaining slots left-to-right, wrapping around from position `r+1`,
   with cities from the **secondary** parent in the order they appear — skipping
   cities already placed.

```
primary  : A B | C D E | F G          segment [2..4]
secondary: C F   A G E   D B

child    : _  _  C  D  E  _  _        copy segment
           fill from secondary, starting at (r+1) % N, skipping {C,D,E}:
           → F, A, G still to place
child    : G  F  C  D  E  A  _        wrap-fill
           → B last
child    : G  F  C  D  E  A  B
```

Two children are produced by swapping parent roles.

### Two-Point Crossover

Identical OX-fill logic but with independently chosen segment endpoints — producing
a wider range of segment lengths for additional diversity.

### Cyclic Crossover (CX)

Traces **cycles** in the mapping between parents.  Cities within the same cycle are
inherited from alternating parents, preserving as many absolute positions as possible.

```
mom : 1  2  3  4  5  6  7  8
dad : 2  4  6  8  7  5  3  1

Cycle starting at pos 0: mom[0]=1 → find 1 in dad (pos 7) → dad[7]=1=mom[0] ✓ cycle done
Assign pos 0 from mom: child1[0]=1, child2[0]=2

Cycle starting at pos 1: 2→pos 0 (already in prev cycle) …
```

Alternating cycles are contributed by alternating parents.

---

## Mutation

Three mutation operators are applied after crossover.  One is chosen at random per
child with equal probability.

| Operator | Probability | Effect |
|----------|-------------|--------|
| **Inversion (2-opt)** | 2/6 | Reverse a random sub-sequence — equivalent to removing two edges and reconnecting |
| **Insert** | 2/6 | Remove one city and re-insert it at a random position |
| **Shuffle** | 2/6 | Randomly permute a random sub-sequence (strong diversification) |

**Inversion** is the most powerful single-step improvement for TSP: reversing
`[c_l, …, c_r]` replaces edges `(c_{l-1}, c_l)` and `(c_r, c_{r+1})` with
`(c_{l-1}, c_r)` and `(c_l, c_{r+1})`, which is exactly a 2-opt swap.

Equal weighting gives shuffle (the most disruptive operator) a larger share than
in a naïve implementation, which helps the population escape local optima.

---

## Elitism

The top `⌊P × ELITISM_PCT⌋` individuals (shortest tours) are carried into the next
generation **unchanged**.  They are excluded from the survivor selection pool and
placed directly into the new population before sorting.

This guarantees the best solution found so far is never lost.

---

## Survivor Selection

After breeding, the new generation is formed by:

1. **Elite slots** — top `eliteCount` individuals from the current population survive unconditionally.
2. **Competition pool** — all remaining (non-elite) incumbents + all offspring.
3. Sort the pool ascending by tour length; keep the best `P − eliteCount` as the non-elite slots.
4. Discard (free) all pool members not selected.
5. Re-sort the full population so `pop[0]` is always the global best.

> **Bug in naive implementations:** evicted chromosomes are never freed, causing a
> memory leak that grows with every generation.

---

## Immigrant Injection

Every `IMMIGRANT_INTERVAL` generations the worst `IMMIGRANT_PCT` of the population
is replaced with **freshly generated random tours**.  This re-introduces diversity
when the population has converged to a basin and crossover can no longer escape it.

```
every 20 generations:
    replace worst 10 % of population with random permutations
    re-sort population
```

Without immigrant injection, a converged population stays stuck: all parents share
the same structure, so every child is a minor variant of the same local optimum.

---

## Termination

The algorithm stops when either condition is met:

| Condition | Value | Meaning |
|-----------|-------|---------|
| `MAX_GEN` | 2 000 | Hard cap on generations |
| `STAGNATION_GEN` | 200 | Consecutive generations where `best` improved by less than 0.01 |

Stagnation tracks whether the **best individual** has actually improved, not whether
the population is diverse.  This prevents premature termination when everyone has
converged to the same (possibly suboptimal) tour.

---

## Parameters

| Parameter | Value | Notes |
|-----------|-------|-------|
| `NEW_GEN_PCT` | 0.60 | 60 % of P generated as offspring each generation |
| `SURVIVE_PCT` | 0.50 | 50 % of P used as parents (more diversity than 0.30) |
| `ELITISM_PCT` | 0.02 | 2 % of P carried unchanged |
| `STAGNATION_GEN` | 200 | Generations without best-improvement before early stop |
| `STAGNATION_EPS` | 0.01 | Minimum improvement in best tour length to reset counter |
| `MAX_GEN` | 2 000 | Hard generation cap |
| `PRINT_EVERY` | 250 | Progress line frequency |
| `IMMIGRANT_PCT` | 0.10 | 10 % of worst individuals replaced with random tours |
| `IMMIGRANT_INTERVAL` | 20 | Immigrant injection every N generations |

---

## Generation Loop Summary

```
initialise P random tours, sort ascending
print generation 0

while generation < MAX_GEN and stagnation < STAGNATION_GEN:
    parents   ← SUS(population, SURVIVE_PCT)              // select ~50 % as parents
    offspring ← breed(parents, NEW_GEN_PCT × P)           // crossover + mutate
    if generation % 20 == 0: inject 10 % random immigrants
    population ← elites ∪ best(non-elites ∪ offspring)   // survivor selection
    sort population ascending
    if generation == 1 or generation % 250 == 0: print best
    if best improved by ≥ 0.01: reset stagnation; else stagnation++

print final generation + best tour
```

---

## Complexity

| Phase | Time | Space |
|-------|------|-------|
| Distance precomputation | O(N²) | O(N²) |
| Population initialisation | O(P · N) | O(P · N) |
| Fitness evaluation | O(N) per individual | — |
| SUS selection | O(P) | O(P) |
| OX / 2-pt / CX crossover | O(N) per child | O(N) |
| Mutation | O(N) worst case | O(1) |
| Survivor sort | O(k log k), k = pool size | O(k) |
| **Per generation** | **O(P · N + k log k)** | **O(P · N)** |
| **Total** | **O(G · P · N)**, G ≤ 2 000 | **O(P · N)** |

---

## UK-12 Benchmark

The `data/` folder contains a 12-city UK dataset with coordinates in a custom
Cartesian system (km-scale offsets, Aberystwyth as approximate origin).

**True optimal tour length: 1872.7806**
verified by Held-Karp exact algorithm (guaranteed global optimum).

```
Aberystwyth → Inverness → Brighton → Newcastle → Oxford → Liverpool →
Exeter → Stratford → London → Edinburgh → Glasgow → Nottingham
```

A correctly implemented GA with P ≥ 300 consistently finds this tour.

> **Note on the value 1595.74:** this figure appears in some academic references
> for a dataset with the same city names but **different (integer grid) coordinates**.
> It does not apply to the CSV files in this repository.

---

## Source Files

| Language | File |
|----------|------|
| C++ | `tsp_ga.cpp` |
| Java | `TspGa.java` |
| Python | `tsp_ga.py` |

All three implement the identical algorithm with the same hyper-parameters and seed.

---

## Build & Run

**C++**
```bash
cd TSP
g++ -std=c++17 -O2 -o tsp_ga tsp_ga.cpp

# random cities
echo "1
20
150" | ./tsp_ga

# UK-12 benchmark
echo "2
150" | ./tsp_ga
```

**Java**
```bash
cd TSP
javac TspGa.java

# random cities
echo "1
20
150" | java TspGa

# UK-12 benchmark
echo "2
150" | java TspGa
```

**Python**
```bash
cd TSP
echo "1
20
150" | python3 tsp_ga.py

echo "2
150" | python3 tsp_ga.py
```

**Input format** (interactive prompts, or piped as above):
```
<mode>      # 1 = random cities, 2 = UK-12 benchmark
<N>         # number of cities (mode 1 only)
<P>         # population size
```
