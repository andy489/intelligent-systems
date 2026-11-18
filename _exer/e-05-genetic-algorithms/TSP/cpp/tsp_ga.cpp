/*
 * Travelling Salesman Problem — Genetic Algorithm
 *
 * Chromosome  : permutation [c0, c1, …, c_{N-1}] — a candidate tour
 * Fitness     : total closed-loop tour length — MINIMISED
 * Selection   : Stochastic Universal Sampling (inverted for minimisation)
 * Crossover   : Order XO (OX), Two-Point XO, Cyclic XO — chosen at random
 * Mutation    : inversion (2-opt), insert, shuffle — chosen at random
 * Elitism     : top ELITISM_PCT individuals bypass survivor selection
 * Termination : MAX_GENERATIONS hard cap or STAGNATION_THRESHOLD generations
 *               with no meaningful improvement
 */

#include <cstdio>
#include <cmath>
#include <cstring>
#include <random>
#include <algorithm>
#include <numeric>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

// ─── ANSI colours ─────────────────────────────────────────────────────────────
static const char *GRN = "\x1B[32m";
static const char *BLU = "\033[0;34m";
static const char *RST = "\033[0m";

// ─── GA hyper-parameters ──────────────────────────────────────────────────────

// Fraction of population generated as offspring each generation.
// Higher → more exploration.
static const double NEW_GEN_PCT   = 0.60;

// Fraction of population used as parents in selection.
static const double SURVIVE_PCT   = 0.50;

// Fraction of population carried unchanged (elite individuals).
static const double ELITISM_PCT   = 0.02;

// Consecutive generations with no improvement in the best tour before early stop.
static const int    STAGNATION_GEN = 200;

// Minimum improvement in best tour length to reset the stagnation counter.
static const double STAGNATION_EPS = 0.01;

// Hard cap on number of generations.
static const int    MAX_GEN        = 2000;

// Print a progress line every this many generations (+ gen 1 + final gen).
static const int    PRINT_EVERY    = 250;

// City coordinate range for random-generation mode.
static const double COORD_LO = 0.0;
static const double COORD_HI = 10000.0;

// ─── Global state ─────────────────────────────────────────────────────────────
static int N;           // number of cities
static int P;           // population size
static int newGenSize;  // = ceil(NEW_GEN_PCT * P), rounded up to be even

static mt19937 rng(42); // fixed seed for reproducibility; change to time(nullptr) for random runs

// City coordinates — stored as double.
// BUG in original: used pair<int,int>, which truncated the UK-12 dataset's
// floating-point coordinates to zero (e.g. 383.458 → 383, -282.758 → -282).
static vector<double> cityX, cityY;
static vector<vector<double>> dist;  // precomputed pairwise Euclidean distances

// Population sorted *ascending* by fitness: pop[0] = shortest (best) tour.
// BUG in original: sorted *descending*, so pop[0] was the worst individual,
// breaking elitism, the stagnation check, and the final result report.
static vector<int *> pop;
static vector<double> fit;  // fit[i] = tour length of pop[i]

// City names loaded from CSV (populated by loadUK12).
static vector<string> cityNames;

// ─── Utilities ────────────────────────────────────────────────────────────────

static inline int    randInt(int lo, int hi)       { return uniform_int_distribution<int>(lo, hi)(rng); }
static inline double randDouble(double lo, double hi) { return uniform_real_distribution<double>(lo, hi)(rng); }

// ─── Fitness ──────────────────────────────────────────────────────────────────

// BUG in original: missing the closing edge c[N-1] → c[0].
// TSP is a *closed* tour; the return leg is part of the cost.
static double tourLength(const int *c) {
    double len = 0.0;
    for (int i = 1; i < N; ++i)
        len += dist[c[i - 1]][c[i]];
    len += dist[c[N - 1]][c[0]];  // close the loop
    return len;
}

// ─── Initialisation ───────────────────────────────────────────────────────────

static void computeDistances() {
    dist.assign(N, vector<double>(N, 0.0));
    for (int i = 0; i < N; ++i)
        for (int j = i + 1; j < N; ++j) {
            double dx = cityX[i] - cityX[j];
            double dy = cityY[i] - cityY[j];
            dist[i][j] = dist[j][i] = sqrt(dx * dx + dy * dy);
        }
}

// Mode 1: randomly generate N cities on [COORD_LO, COORD_HI]^2.
static void generateRandomCities() {
    printf("%sEnter N (number of cities, N ≤ 100): %s", BLU, RST);
    scanf("%d", &N);

    cityX.resize(N);
    cityY.resize(N);
    for (int i = 0; i < N; ++i) {
        cityX[i] = randDouble(COORD_LO, COORD_HI);
        cityY[i] = randDouble(COORD_LO, COORD_HI);
    }
    computeDistances();
}

// Mode 2: load UK-12 dataset from CSV files in ../data/.
// Reads uk12_name.csv (one city name per line) and uk12_xy.csv (x,y per line).
static void loadUK12() {
    const char *nameFile = "../data/uk12_name.csv";
    const char *xyFile   = "../data/uk12_xy.csv";

    // Read names
    ifstream nf(nameFile);
    if (!nf) { fprintf(stderr, "Cannot open %s\n", nameFile); exit(1); }
    string line;
    while (getline(nf, line)) {
        if (!line.empty()) cityNames.push_back(line);
    }

    // Read coordinates
    ifstream xf(xyFile);
    if (!xf) { fprintf(stderr, "Cannot open %s\n", xyFile); exit(1); }
    while (getline(xf, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string xs, ys;
        getline(ss, xs, ',');
        getline(ss, ys, ',');
        cityX.push_back(stod(xs));
        cityY.push_back(stod(ys));
    }

    if (cityNames.size() != cityX.size()) {
        fprintf(stderr, "Mismatch: %zu names vs %zu coordinates\n",
                cityNames.size(), cityX.size());
        exit(1);
    }

    N = (int)cityX.size();
    computeDistances();
}

static void initPopulation() {
    printf("%sEnter P (population size, e.g. 150): %s", BLU, RST);
    scanf("%d", &P);

    newGenSize = (int)round(NEW_GEN_PCT * P);
    if (newGenSize % 2) ++newGenSize;  // keep even so children come in pairs
    newGenSize = max(2, newGenSize);

    pop.resize(P);
    fit.resize(P);

    vector<int> perm(N);
    iota(perm.begin(), perm.end(), 0);

    for (int i = 0; i < P; ++i) {
        pop[i] = new int[N];
        shuffle(perm.begin(), perm.end(), rng);
        copy(perm.begin(), perm.end(), pop[i]);
        fit[i] = tourLength(pop[i]);
    }

    // Sort ascending so pop[0] is always the best (shortest) individual.
    vector<int> idx(P);
    iota(idx.begin(), idx.end(), 0);
    sort(idx.begin(), idx.end(), [](int a, int b) { return fit[a] < fit[b]; });

    vector<int *>  tmpP(P);
    vector<double> tmpF(P);
    for (int i = 0; i < P; ++i) { tmpP[i] = pop[idx[i]]; tmpF[i] = fit[idx[i]]; }
    pop = tmpP;
    fit = tmpF;
}

// ─── Crossover operators ──────────────────────────────────────────────────────

// Order Crossover (OX) — canonical permutation XO for TSP.
// Copies a random segment verbatim from one parent; remaining positions are
// filled left-to-right in the order they appear in the other parent.
static pair<int *, int *> orderCrossover(const int *mom, const int *dad) {
    auto ox = [](int *child, const int *primary, const int *secondary, int n) {
        int l = randInt(0, n - 1);
        int r = randInt(0, n - 1);
        if (l > r) swap(l, r);

        fill(child, child + n, -1);
        vector<bool> used(n, false);

        for (int i = l; i <= r; ++i) {
            child[i] = primary[i];
            used[primary[i]] = true;
        }

        int pos = (r + 1) % n;
        for (int i = 0; i < n; ++i) {
            int city = secondary[(r + 1 + i) % n];
            if (!used[city]) {
                child[pos] = city;
                used[city] = true;
                pos = (pos + 1) % n;
            }
        }
    };

    int *c1 = new int[N];
    int *c2 = new int[N];
    ox(c1, mom, dad, N);
    ox(c2, dad, mom, N);  // symmetric: swap parent roles
    return {c1, c2};
}

// Two-Point Crossover with OX-style repair.
// Copies the middle segment from primary; fills outside from secondary in order.
// BUG in original: the wrapping/fill logic could desync pointers and produce
// chromosomes with duplicate or missing cities on edge cases.
static pair<int *, int *> twoPointCrossover(const int *mom, const int *dad) {
    auto tpx = [](int *child, const int *primary, const int *secondary, int n) {
        int l = randInt(0, n - 1);
        int r = randInt(0, n - 1);
        if (l > r) swap(l, r);

        fill(child, child + n, -1);
        vector<bool> used(n, false);

        for (int i = l; i <= r; ++i) {
            child[i] = primary[i];
            used[primary[i]] = true;
        }

        int pos = (r + 1) % n;
        for (int i = 0; i < n; ++i) {
            int city = secondary[(r + 1 + i) % n];
            if (!used[city]) {
                child[pos] = city;
                used[city] = true;
                pos = (pos + 1) % n;
            }
        }
    };

    int *c1 = new int[N];
    int *c2 = new int[N];
    tpx(c1, mom, dad, N);
    tpx(c2, dad, mom, N);
    return {c1, c2};
}

// Cyclic Crossover (CX) — preserves absolute city positions where possible.
static pair<int *, int *> cyclicCrossover(const int *mom, const int *dad) {
    // posInMom[city] = the index at which that city appears in mom
    vector<int> posInMom(N);
    for (int i = 0; i < N; ++i)
        posInMom[mom[i]] = i;

    int *c1 = new int[N];
    int *c2 = new int[N];
    fill(c1, c1 + N, -1);
    fill(c2, c2 + N, -1);

    bool copyMom = true;
    for (int i = 0; i < N; ++i) {
        if (c1[i] != -1) continue;  // already assigned by a previous cycle

        // Trace the full cycle starting at position i
        int cur = i;
        while (c1[cur] == -1) {
            c1[cur] = copyMom ? mom[cur] : dad[cur];
            c2[cur] = copyMom ? dad[cur] : mom[cur];
            cur = posInMom[dad[cur]];
        }
        copyMom = !copyMom;
    }

    return {c1, c2};
}

// ─── Mutation operators ───────────────────────────────────────────────────────

// Inversion (2-opt): reverse a random sub-sequence.
// Most effective single-step local move for TSP.
static void inversionMutation(int *c) {
    int l = randInt(0, N - 1);
    int r = randInt(0, N - 1);
    if (l == r) return;
    if (l > r) swap(l, r);
    reverse(c + l, c + r + 1);
}

// Insert: remove one city and re-insert it at a different position.
static void insertMutation(int *c) {
    int from = randInt(0, N - 1);
    int to   = randInt(0, N - 1);
    if (from == to) return;
    int city = c[from];
    if (from < to) for (int i = from; i < to;   ++i) c[i] = c[i + 1];
    else           for (int i = from; i > to;   --i) c[i] = c[i - 1];
    c[to] = city;
}

// Shuffle: randomise a random sub-sequence (strong diversification).
static void shuffleMutation(int *c) {
    int l = randInt(0, N - 1);
    int r = randInt(0, N - 1);
    if (l > r) swap(l, r);
    shuffle(c + l, c + r + 1, rng);
}

// Mutation weights: inversion 2/6, insert 2/6, shuffle 2/6.
// Equal shuffle weight helps escape local optima that inversion alone cannot.
static void mutate(int *c) {
    switch (randInt(1, 6)) {
        case 1: case 2: inversionMutation(c); break;
        case 3: case 4: insertMutation(c);    break;
        case 5: case 6: shuffleMutation(c);   break;
    }
}

// ─── Selection ────────────────────────────────────────────────────────────────

// Stochastic Universal Sampling adapted for MINIMISATION.
//
// BUG in original: raw fitness (= distance) was divided by totalFitness, so
// individuals with *longer* (worse) tours received *larger* probability slices.
// Selection pressure was completely inverted.
//
// Fix: invert fitness values so shorter tours get more weight.
// inv[i] = maxFit - fit[i] + 1  (the +1 ensures the worst still gets weight > 0)
static vector<int> stochasticUniversalSampling() {
    int sampleSize = max(2, (int)(P * SURVIVE_PCT));

    double maxFit = *max_element(fit.begin(), fit.end());
    double total  = 0.0;
    vector<double> inv(P);
    for (int i = 0; i < P; ++i) {
        inv[i] = maxFit - fit[i] + 1.0;
        total += inv[i];
    }

    // Cumulative distribution over inverted weights
    vector<double> cum(P);
    cum[0] = inv[0] / total;
    for (int i = 1; i < P; ++i)
        cum[i] = cum[i - 1] + inv[i] / total;
    cum[P - 1] = 1.0;  // clamp to avoid floating-point overshoot

    double step  = 1.0 / sampleSize;
    double start = randDouble(0.0, step);

    vector<int> selected;
    selected.reserve(sampleSize);
    int j = 0;
    for (int k = 0; k < sampleSize; ++k) {
        double point = start + k * step;
        while (j < P - 1 && cum[j] < point) ++j;
        selected.push_back(j);
    }

    return selected;
}

// ─── Breeding ─────────────────────────────────────────────────────────────────

// Generate newGenSize offspring by randomly pairing selected parents, applying
// a randomly chosen crossover and then mutation.
static vector<int *> breed(const vector<int> &selected) {
    vector<int *> offspring;
    offspring.reserve(newGenSize);

    int s = (int)selected.size();
    while ((int)offspring.size() < newGenSize) {
        const int *mom = pop[selected[randInt(0, s - 1)]];
        const int *dad = pop[selected[randInt(0, s - 1)]];

        pair<int *, int *> children;
        switch (randInt(1, 6)) {
            case 1: case 2: case 3: children = orderCrossover(mom, dad);    break;
            case 4: case 5:         children = twoPointCrossover(mom, dad); break;
            case 6:                 children = cyclicCrossover(mom, dad);   break;
        }

        mutate(children.first);
        mutate(children.second);

        offspring.push_back(children.first);
        offspring.push_back(children.second);
    }

    return offspring;
}

// ─── Survivor selection ───────────────────────────────────────────────────────

// Merge elite incumbents + remaining population + offspring, then keep the
// best P individuals.  Elite individuals (pop[0..eliteCount-1]) are preserved
// unconditionally and are not part of the competition pool.
//
// BUGs in original:
//  • Off-by-one in the split between elites and competitors.
//  • Chromosomes evicted from the population were never freed (memory leak).
//  • The sort/insert logic for elitism was incorrect when an elite slot got
//    displaced during insertion sort.
static void updatePopulation(vector<int *> &offspring) {
    int eliteCount  = max(1, (int)(P * ELITISM_PCT));
    int nonElite    = P - eliteCount;
    int poolSize    = nonElite + (int)offspring.size();

    // Build the competition pool from non-elite incumbents + all offspring
    vector<int *>  pool(poolSize);
    vector<double> poolFit(poolSize);

    for (int i = 0; i < nonElite; ++i) {
        pool[i]    = pop[eliteCount + i];
        poolFit[i] = fit[eliteCount + i];
    }
    for (int i = 0; i < (int)offspring.size(); ++i) {
        pool[nonElite + i]    = offspring[i];
        poolFit[nonElite + i] = tourLength(offspring[i]);
    }

    // Select the best `nonElite` from the pool (sort ascending, keep front)
    vector<int> order(poolSize);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(),
         [&](int a, int b) { return poolFit[a] < poolFit[b]; });

    vector<bool> kept(poolSize, false);
    for (int i = 0; i < nonElite; ++i) {
        pop[eliteCount + i] = pool[order[i]];
        fit[eliteCount + i] = poolFit[order[i]];
        kept[order[i]] = true;
    }

    // Free every chromosome in the pool that was NOT selected
    for (int i = 0; i < poolSize; ++i)
        if (!kept[i])
            delete[] pool[i];

    // Re-sort the whole population so pop[0] is always the global best
    vector<int> idx(P);
    iota(idx.begin(), idx.end(), 0);
    sort(idx.begin(), idx.end(), [](int a, int b) { return fit[a] < fit[b]; });

    vector<int *>  tmpP(P);
    vector<double> tmpF(P);
    for (int i = 0; i < P; ++i) { tmpP[i] = pop[idx[i]]; tmpF[i] = fit[idx[i]]; }
    pop = tmpP;
    fit = tmpF;
}

// ─── Immigrant injection ─────────────────────────────────────────────────────

// Replace the worst IMMIGRANT_PCT of the population with fresh random tours.
// Fires every IMMIGRANT_INTERVAL generations to re-introduce diversity when
// the population has converged to a local optimum basin.
static const double IMMIGRANT_PCT      = 0.10;
static const int    IMMIGRANT_INTERVAL = 20;

static void injectImmigrants() {
    int count = max(1, (int)(P * IMMIGRANT_PCT));
    // Replace the worst `count` individuals (highest indices, pop is sorted ascending).
    vector<int> perm(N);
    iota(perm.begin(), perm.end(), 0);
    for (int i = P - count; i < P; ++i) {
        shuffle(perm.begin(), perm.end(), rng);
        copy(perm.begin(), perm.end(), pop[i]);
        fit[i] = tourLength(pop[i]);
    }
    // Re-sort so pop[0] stays the best.
    vector<int> idx(P);
    iota(idx.begin(), idx.end(), 0);
    sort(idx.begin(), idx.end(), [](int a, int b) { return fit[a] < fit[b]; });
    vector<int *>  tmpP(P);
    vector<double> tmpF(P);
    for (int i = 0; i < P; ++i) { tmpP[i] = pop[idx[i]]; tmpF[i] = fit[idx[i]]; }
    pop = tmpP;
    fit = tmpF;
}

// ─── Main GA loop ─────────────────────────────────────────────────────────────

static void evolve(bool useUK12) {
    int stagnation  = 0;
    int generations = 0;
    double prevBest = fit[0];

    auto printBest = [&]() {
        printf("Gen %4d: %sbest = %.2f%s\n", generations, GRN, fit[0], RST);
    };

    printBest();  // baseline: initial population (generation 0)

    while (generations < MAX_GEN && stagnation < STAGNATION_GEN) {
        ++generations;

        vector<int> parents  = stochasticUniversalSampling();
        vector<int *> children = breed(parents);
        updatePopulation(children);

        // Periodically inject random immigrants to escape local optima basins.
        if (generations % IMMIGRANT_INTERVAL == 0)
            injectImmigrants();

        if (generations == 1 || generations % PRINT_EVERY == 0)
            printBest();

        // Stagnation = best tour has not improved by at least STAGNATION_EPS.
        if (prevBest - fit[0] >= STAGNATION_EPS) {
            stagnation = 0;
            prevBest   = fit[0];
        } else {
            ++stagnation;
        }
    }

    // Final progress line (mandatory per problem statement)
    printf("Gen %4d: %sbest = %.2f%s  (final)\n", generations, GRN, fit[0], RST);
    printf("\n%sTotal generations: %d%s\n", BLU, generations, RST);

    printf("Best tour length: %.4f\n", fit[0]);
    if (useUK12) {
        printf("Best tour: ");
        for (int i = 0; i < N; ++i)
            printf("%s%s", cityNames[pop[0][i]].c_str(), (i < N - 1) ? " → " : "\n");
        printf("Known optimal (these coords): ~1872.78\n");
    } else {
        printf("Best tour: ");
        for (int i = 0; i < N; ++i)
            printf("%d%s", pop[0][i], (i < N - 1) ? " → " : "\n");
    }

    for (int i = 0; i < P; ++i)
        delete[] pop[i];
    pop.clear();
}

// ─── Entry point ─────────────────────────────────────────────────────────────

int main() {
    printf("%s=== Travelling Salesman Problem — Genetic Algorithm ===%s\n\n", BLU, RST);
    printf("Mode: (1) random cities  (2) UK-12 benchmark\nChoice: ");

    int mode = 1;
    scanf("%d", &mode);

    bool useUK12 = (mode == 2);
    if (useUK12) loadUK12();
    else         generateRandomCities();

    initPopulation();
    evolve(useUK12);

    return 0;
}
