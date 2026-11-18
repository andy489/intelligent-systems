# Problem — Travelling Salesman Problem via Genetic Algorithm

## Statement

Solve the **Travelling Salesman Problem (TSP)** using a Genetic Algorithm.

The program is given **N** (N ≤ 100) — the number of points (cities) in 2D space.  
The program **randomly generates** N points in a 2D coordinate system.

Find the **shortest path** that passes through every point **exactly once**.

## Progress Output Requirement

At **at least 5 steps**, print the length of the current best tour in the population:

1. After generation 1
2. (intermediate — your choice)
3. (intermediate — your choice)
4. (intermediate — your choice)
5. After the last generation

## Formal GA Definition

| Component | Definition |
|-----------|-----------|
| **Individual (chromosome)** | A permutation of city indices `[c₀, c₁, …, c_{N−1}]` — a candidate tour |
| **Population** | A set of P individuals evolved across generations |
| **Fitness function** | Total tour length: `f(tour) = Σ dist(cᵢ, c_{i+1}) + dist(c_{N−1}, c₀)` — minimised |
| **Selection** | Tournament or roulette-wheel selection; fitter individuals are more likely to reproduce |
| **Crossover** | Order Crossover (OX) — preserves relative order of cities from both parents |
| **Mutation** | Swap or inversion mutation — randomly alters a tour to maintain diversity |
| **Termination** | Fixed number of generations |

## Input / Output

**Input:** A single integer N (N ≤ 100) — number of cities.

**Program behaviour:**
- Randomly generate N points `(x, y)` in 2D space
- Run the genetic algorithm
- Print best tour length at generation 1, at least 3 intermediate generations, and after the final generation
- Print the best tour found (city visit order)

**Example output:**
```
Gen    1: best = 342.57
Gen  250: best = 198.34
Gen  500: best = 171.22
Gen  750: best = 163.89
Gen 1000: best = 161.45
Best tour: 0 4 2 7 1 5 3 6
```

## GA Parameters (suggested)

| Parameter | Typical value |
|-----------|--------------|
| Population size P | 100 – 200 |
| Generations | 500 – 2 000 |
| Crossover rate | 0.8 – 0.95 |
| Mutation rate | 0.01 – 0.05 |
| Elitism | Keep top 1–2 individuals unchanged |

## Special Cases

| N | Expected behaviour |
|---|-------------------|
| 1 | Single city — tour length 0 |
| 2 | Only one tour — trivially optimal |
| N ≥ 3 | GA evolves a near-optimal permutation |

## Benchmark Dataset — UK 12 Cities

A reference dataset of 12 UK cities is provided in [`data/`](data/) for validating implementations.

| File | Contents |
|------|----------|
| [`uk12_name.csv`](data/uk12_name.csv) | City names (one per line, alphabetical order) |
| [`uk12_xy.csv`](data/uk12_xy.csv) | City coordinates `(x, y)`, same order as names |
| [`cities.png`](data/cities.png) | Plot of the 12 cities |
| [`tour.png`](data/tour.png) | Plot of the known optimal tour |

**Known optimal tour** (distance = **1595.7385**):

```
Aberystwyth → Inverness → Nottingham → Glasgow → Edinburgh →
London → Stratford → Exeter → Liverpool → Oxford → Brighton → Newcastle
```

A correct GA implementation should converge to, or very close to, this tour on the UK12 dataset.
