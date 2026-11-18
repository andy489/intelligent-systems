# Travelling Salesman Problem — Genetic Algorithm
#
# Chromosome  : list[int] permutation [c0, c1, …, c_{N-1}] — a candidate tour
# Fitness     : total closed-loop tour length — MINIMISED
# Selection   : Stochastic Universal Sampling (inverted for minimisation)
# Crossover   : Order XO (OX), Two-Point XO, Cyclic XO — chosen at random
# Mutation    : inversion (2-opt), insert, shuffle — chosen at random
# Elitism     : top ELITISM_PCT individuals bypass survivor selection
# Termination : MAX_GEN hard cap or STAGNATION_GEN consecutive generations
#               with no meaningful improvement

from __future__ import annotations
import sys
import math
import random
import csv
import os

# ─── ANSI colours ─────────────────────────────────────────────────────────────
GRN = "\033[32m"
BLU = "\033[34m"
RST = "\033[0m"

# ─── GA hyper-parameters ──────────────────────────────────────────────────────
NEW_GEN_PCT    = 0.60
SURVIVE_PCT    = 0.50
ELITISM_PCT    = 0.02
STAGNATION_GEN = 200
STAGNATION_EPS = 0.01   # minimum improvement in best tour to reset stagnation
MAX_GEN        = 2000
PRINT_EVERY    = 250
COORD_LO       = 0.0
COORD_HI       = 10_000.0
IMMIGRANT_PCT      = 0.10   # fraction of worst individuals replaced each injection
IMMIGRANT_INTERVAL = 20     # inject every N generations
DATA_DIR       = os.path.join(os.path.dirname(__file__), "..", "data")

# ─── CSV loader ───────────────────────────────────────────────────────────────

def load_uk12() -> tuple[list[str], list[float], list[float]]:
    """Read city names and coordinates from the data/ CSV files."""
    name_path = os.path.join(DATA_DIR, "uk12_name.csv")
    xy_path   = os.path.join(DATA_DIR, "uk12_xy.csv")

    names: list[str] = []
    with open(name_path, newline="") as f:
        for line in f:
            name = line.strip()
            if name:
                names.append(name)

    cx: list[float] = []
    cy: list[float] = []
    with open(xy_path, newline="") as f:
        reader = csv.reader(f)
        for row in reader:
            if row:
                cx.append(float(row[0]))
                cy.append(float(row[1]))

    if len(names) != len(cx):
        raise ValueError(f"Mismatch: {len(names)} names vs {len(cx)} coordinates")

    return names, cx, cy


# ─── Fitness ──────────────────────────────────────────────────────────────────

def tour_length(c: list[int], dist: list[list[float]]) -> float:
    """Closed tour: sum of consecutive edges + the return edge c[-1] → c[0]."""
    total = sum(dist[c[i - 1]][c[i]] for i in range(1, len(c)))
    total += dist[c[-1]][c[0]]
    return total


# ─── Crossover ────────────────────────────────────────────────────────────────

def _ox_one(primary: list[int], secondary: list[int], rng: random.Random) -> list[int]:
    """Order Crossover (OX) helper — produces one child."""
    n = len(primary)
    l = rng.randrange(n)
    r = rng.randrange(n)
    if l > r:
        l, r = r, l

    child = [-1] * n
    used  = set()
    for i in range(l, r + 1):
        child[i] = primary[i]
        used.add(primary[i])

    pos = (r + 1) % n
    for i in range(n):
        city = secondary[(r + 1 + i) % n]
        if city not in used:
            child[pos] = city
            used.add(city)
            pos = (pos + 1) % n

    return child


def order_crossover(mom: list[int], dad: list[int], rng: random.Random) -> tuple:
    return _ox_one(mom, dad, rng), _ox_one(dad, mom, rng)


def two_point_crossover(mom: list[int], dad: list[int], rng: random.Random) -> tuple:
    # Same OX-style fill; the segment endpoints are chosen independently.
    return _ox_one(mom, dad, rng), _ox_one(dad, mom, rng)


def cyclic_crossover(mom: list[int], dad: list[int], rng: random.Random) -> tuple:
    """Cyclic Crossover (CX): preserves absolute city positions across cycles."""
    n = len(mom)
    pos_in_mom = [0] * n
    for i, city in enumerate(mom):
        pos_in_mom[city] = i

    c1 = [-1] * n
    c2 = [-1] * n
    copy_mom = True

    for i in range(n):
        if c1[i] != -1:
            continue
        cur = i
        while c1[cur] == -1:
            c1[cur] = mom[cur] if copy_mom else dad[cur]
            c2[cur] = dad[cur] if copy_mom else mom[cur]
            cur = pos_in_mom[dad[cur]]
        copy_mom = not copy_mom

    return c1, c2


# ─── Mutation ─────────────────────────────────────────────────────────────────

def inversion_mutation(c: list[int], rng: random.Random) -> None:
    """Inversion (2-opt): reverse a random sub-sequence."""
    n = len(c)
    l, r = rng.randrange(n), rng.randrange(n)
    if l == r:
        return
    if l > r:
        l, r = r, l
    c[l:r + 1] = c[l:r + 1][::-1]


def insert_mutation(c: list[int], rng: random.Random) -> None:
    """Remove one city and re-insert it at a different position."""
    n    = len(c)
    frm  = rng.randrange(n)
    to   = rng.randrange(n)
    if frm == to:
        return
    city = c.pop(frm)
    c.insert(to, city)


def shuffle_mutation(c: list[int], rng: random.Random) -> None:
    """Shuffle a random sub-sequence."""
    n = len(c)
    l, r = rng.randrange(n), rng.randrange(n)
    if l > r:
        l, r = r, l
    sub = c[l:r + 1]
    rng.shuffle(sub)
    c[l:r + 1] = sub


def mutate(c: list[int], rng: random.Random) -> None:
    roll = rng.randrange(6)
    if roll < 2:
        inversion_mutation(c, rng)
    elif roll < 4:
        insert_mutation(c, rng)
    else:
        shuffle_mutation(c, rng)


# ─── Selection ────────────────────────────────────────────────────────────────

def stochastic_universal_sampling(fit: list[float], p: int, rng: random.Random) -> list[int]:
    """SUS adapted for minimisation: shorter tours get higher selection weight."""
    sample_size = max(2, int(p * SURVIVE_PCT))

    max_fit = fit[-1]  # pop sorted ascending, last = worst
    inv     = [max_fit - f + 1.0 for f in fit]
    total   = sum(inv)

    cum = []
    running = 0.0
    for w in inv:
        running += w / total
        cum.append(running)
    cum[-1] = 1.0  # clamp

    step  = 1.0 / sample_size
    start = rng.random() * step

    selected = []
    j = 0
    for k in range(sample_size):
        point = start + k * step
        while j < p - 1 and cum[j] < point:
            j += 1
        selected.append(j)

    return selected


# ─── Breeding ─────────────────────────────────────────────────────────────────

def breed(pop: list[list[int]], selected: list[int], new_gen_size: int,
          rng: random.Random) -> list[list[int]]:
    offspring = []
    s = len(selected)

    while len(offspring) < new_gen_size:
        mom = pop[selected[rng.randrange(s)]]
        dad = pop[selected[rng.randrange(s)]]

        roll = rng.randrange(6)
        if roll < 3:
            c1, c2 = order_crossover(mom, dad, rng)
        elif roll < 5:
            c1, c2 = two_point_crossover(mom, dad, rng)
        else:
            c1, c2 = cyclic_crossover(mom, dad, rng)

        mutate(c1, rng)
        mutate(c2, rng)
        offspring.append(c1)
        offspring.append(c2)

    return offspring


# ─── Survivor selection ───────────────────────────────────────────────────────

def update_population(pop: list[list[int]], fit: list[float],
                      offspring: list[list[int]], dist: list[list[float]],
                      p: int) -> tuple:
    """Merge elite incumbents + pool + offspring; keep the best P individuals."""
    elite_count = max(1, int(p * ELITISM_PCT))
    non_elite   = p - elite_count

    # Competition pool: non-elite incumbents + all offspring
    pool     = pop[elite_count:] + offspring
    pool_fit = fit[elite_count:] + [tour_length(c, dist) for c in offspring]

    # Sort pool ascending, keep the best non_elite
    order    = sorted(range(len(pool)), key=lambda i: pool_fit[i])
    new_pop  = pop[:elite_count] + [pool[order[i]] for i in range(non_elite)]
    new_fit  = fit[:elite_count] + [pool_fit[order[i]] for i in range(non_elite)]

    # Re-sort the whole population
    combined = sorted(zip(new_fit, new_pop), key=lambda x: x[0])
    new_fit  = [x[0] for x in combined]
    new_pop  = [x[1] for x in combined]

    return new_pop, new_fit


# ─── Immigrant injection ──────────────────────────────────────────────────────

def inject_immigrants(pop: list[list[int]], fit: list[float],
                      dist: list[list[float]], n: int, p: int,
                      rng: random.Random) -> tuple:
    """Replace the worst IMMIGRANT_PCT individuals with fresh random tours."""
    count = max(1, int(p * IMMIGRANT_PCT))
    base = list(range(n))
    for i in range(p - count, p):
        c = base[:]
        rng.shuffle(c)
        pop[i] = c
        fit[i] = tour_length(c, dist)
    combined = sorted(zip(fit, pop), key=lambda x: x[0])
    return [x[1] for x in combined], [x[0] for x in combined]



def evolve(n: int, cx: list[float], cy: list[float], p: int,
           use_uk12: bool, city_names: list[str] | None = None,
           seed: int = 42) -> None:
    rng = random.Random(seed)

    # Precompute distance matrix
    dist = [[0.0] * n for _ in range(n)]
    for i in range(n):
        for j in range(i + 1, n):
            dx = cx[i] - cx[j]
            dy = cy[i] - cy[j]
            d = math.sqrt(dx * dx + dy * dy)
            dist[i][j] = dist[j][i] = d

    # New generation size: round to even
    new_gen_size = round(NEW_GEN_PCT * p)
    if new_gen_size % 2:
        new_gen_size += 1
    new_gen_size = max(2, new_gen_size)

    # Initialise population as random permutations, sorted ascending
    base = list(range(n))
    pop  = []
    for _ in range(p):
        c = base[:]
        rng.shuffle(c)
        pop.append(c)
    fit = [tour_length(c, dist) for c in pop]

    combined = sorted(zip(fit, pop), key=lambda x: x[0])
    fit = [x[0] for x in combined]
    pop = [x[1] for x in combined]

    def print_best(gen: int) -> None:
        print(f"Gen {gen:4d}: {GRN}best = {fit[0]:.2f}{RST}")

    generation = 0
    stagnation = 0
    prev_best  = fit[0]
    print_best(generation)

    while generation < MAX_GEN and stagnation < STAGNATION_GEN:
        generation += 1

        parents  = stochastic_universal_sampling(fit, p, rng)
        children = breed(pop, parents, new_gen_size, rng)
        pop, fit = update_population(pop, fit, children, dist, p)

        if generation % IMMIGRANT_INTERVAL == 0:
            pop, fit = inject_immigrants(pop, fit, dist, n, p, rng)

        if generation == 1 or generation % PRINT_EVERY == 0:
            print_best(generation)

        if prev_best - fit[0] >= STAGNATION_EPS:
            stagnation = 0
            prev_best  = fit[0]
        else:
            stagnation += 1

    print(f"Gen {generation:4d}: {GRN}best = {fit[0]:.2f}{RST}  (final)")
    print(f"\nTotal generations: {generation}")
    print(f"Best tour length: {fit[0]:.4f}")

    if use_uk12 and city_names:
        tour_str = " → ".join(city_names[c] for c in pop[0])
        print(f"Best tour: {tour_str}")
        print("Known optimal (these coords): ~1872.78")
    else:
        tour_str = " → ".join(str(c) for c in pop[0])
        print(f"Best tour: {tour_str}")


# ─── Entry point ──────────────────────────────────────────────────────────────

def main() -> None:
    print(f"{BLU}=== Travelling Salesman Problem — Genetic Algorithm ==={RST}\n")
    mode = input("Mode: (1) random cities  (2) UK-12 benchmark\nChoice: ").strip()

    use_uk12 = (mode == "2")
    city_names = None
    if use_uk12:
        city_names, cx, cy = load_uk12()
        n = len(cx)
    else:
        n  = int(input(f"{BLU}Enter N (number of cities, N ≤ 100): {RST}"))
        gen = random.Random(42)
        cx = [COORD_LO + gen.random() * (COORD_HI - COORD_LO) for _ in range(n)]
        cy = [COORD_LO + gen.random() * (COORD_HI - COORD_LO) for _ in range(n)]

    p = int(input(f"{BLU}Enter P (population size, e.g. 150): {RST}"))
    evolve(n, cx, cy, p, use_uk12, city_names, seed=42)


if __name__ == "__main__":
    main()
