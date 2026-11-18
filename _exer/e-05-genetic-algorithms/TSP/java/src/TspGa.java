// Travelling Salesman Problem — Genetic Algorithm
//
// Chromosome  : int[] permutation [c0, c1, …, c_{N-1}] — a candidate tour
// Fitness     : total closed-loop tour length — MINIMISED
// Selection   : Stochastic Universal Sampling (inverted for minimisation)
// Crossover   : Order XO (OX), Two-Point XO, Cyclic XO — chosen at random
// Mutation    : inversion (2-opt), insert, shuffle — chosen at random
// Elitism     : top ELITISM_PCT individuals bypass survivor selection
// Termination : MAX_GEN hard cap or STAGNATION_GEN consecutive generations
//               with no meaningful improvement

import java.util.*;
import java.io.*;
import java.nio.file.*;

public class TspGa {

    // ─── ANSI colours ──────────────────────────────────────────────────────────
    private static final String GRN = "\u001B[32m";
    private static final String BLU = "\u001B[34m";
    private static final String RST = "\u001B[0m";

    // ─── GA hyper-parameters ───────────────────────────────────────────────────
    private static final double NEW_GEN_PCT    = 0.60;
    private static final double SURVIVE_PCT    = 0.50;
    private static final double ELITISM_PCT    = 0.02;
    private static final int    STAGNATION_GEN = 200;
    private static final double STAGNATION_EPS = 0.01;  // min improvement to reset stagnation
    private static final int    MAX_GEN        = 2000;
    private static final int    PRINT_EVERY    = 250;
    private static final double COORD_LO       = 0.0;
    private static final double COORD_HI       = 10_000.0;
    private static final double IMMIGRANT_PCT      = 0.10;
    private static final int    IMMIGRANT_INTERVAL = 20;

    private static final String DATA_DIR = "../data/";

    // ─── State ─────────────────────────────────────────────────────────────────
    private final int      n;           // number of cities
    private final int      p;           // population size
    private final int      newGenSize;  // offspring count per generation
    private final double[] cx, cy;      // city coordinates
    private final String[] cityNames;   // loaded from CSV (null for random mode)
    private final double[][] dist;      // precomputed pairwise distances
    private final Random   rng;
    private final boolean  useUK12;

    // Population: sorted ascending by tour length (index 0 = best).
    private int[][]  pop;
    private double[] fit;

    // ─── Construction ──────────────────────────────────────────────────────────

    private TspGa(int n, double[] cx, double[] cy, String[] cityNames,
                  int p, boolean useUK12, long seed) {
        this.n         = n;
        this.p         = p;
        this.cx        = cx;
        this.cy        = cy;
        this.cityNames = cityNames;
        this.useUK12   = useUK12;
        this.rng       = new Random(seed);

        int ngs = (int) Math.round(NEW_GEN_PCT * p);
        if (ngs % 2 != 0) ngs++;
        this.newGenSize = Math.max(2, ngs);

        this.dist = new double[n][n];
        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++) {
                double dx = cx[i] - cx[j];
                double dy = cy[i] - cy[j];
                dist[i][j] = dist[j][i] = Math.sqrt(dx * dx + dy * dy);
            }

        initPopulation();
    }

    // ─── Fitness ───────────────────────────────────────────────────────────────

    // Closed tour: sum of consecutive edges + the return edge c[N-1] → c[0].
    private double tourLength(int[] c) {
        double len = 0.0;
        for (int i = 1; i < n; i++)
            len += dist[c[i - 1]][c[i]];
        len += dist[c[n - 1]][c[0]];
        return len;
    }

    // ─── Initialisation ────────────────────────────────────────────────────────

    private void initPopulation() {
        pop = new int[p][n];
        fit = new double[p];

        int[] perm = new int[n];
        for (int i = 0; i < n; i++) perm[i] = i;

        for (int i = 0; i < p; i++) {
            shuffleArray(perm);
            pop[i] = Arrays.copyOf(perm, n);
            fit[i] = tourLength(pop[i]);
        }

        sortByFitness();
    }

    // Sort pop/fit together ascending by tour length (best = index 0).
    private void sortByFitness() {
        Integer[] idx = new Integer[p];
        for (int i = 0; i < p; i++) idx[i] = i;
        Arrays.sort(idx, (a, b) -> Double.compare(fit[a], fit[b]));

        int[][]  tmpP = new int[p][];
        double[] tmpF = new double[p];
        for (int i = 0; i < p; i++) {
            tmpP[i] = pop[idx[i]];
            tmpF[i] = fit[idx[i]];
        }
        pop = tmpP;
        fit = tmpF;
    }

    // ─── Crossover ─────────────────────────────────────────────────────────────

    // Order Crossover (OX): copy a random segment from primary; fill the rest
    // left-to-right in the order cities appear in secondary.
    private int[] ox(int[] primary, int[] secondary) {
        int l = rng.nextInt(n);
        int r = rng.nextInt(n);
        if (l > r) { int t = l; l = r; r = t; }

        int[] child = new int[n];
        Arrays.fill(child, -1);
        boolean[] used = new boolean[n];

        for (int i = l; i <= r; i++) {
            child[i] = primary[i];
            used[primary[i]] = true;
        }

        int pos = (r + 1) % n;
        for (int i = 0; i < n; i++) {
            int city = secondary[(r + 1 + i) % n];
            if (!used[city]) {
                child[pos] = city;
                used[city] = true;
                pos = (pos + 1) % n;
            }
        }
        return child;
    }

    private int[][] orderCrossover(int[] mom, int[] dad) {
        return new int[][] { ox(mom, dad), ox(dad, mom) };
    }

    // Two-Point Crossover: same OX-style fill but with a different segment choice.
    private int[][] twoPointCrossover(int[] mom, int[] dad) {
        return new int[][] { ox(mom, dad), ox(dad, mom) };
    }

    // Cyclic Crossover (CX): preserves absolute city positions across cycles.
    private int[][] cyclicCrossover(int[] mom, int[] dad) {
        int[] posInMom = new int[n];
        for (int i = 0; i < n; i++) posInMom[mom[i]] = i;

        int[] c1 = new int[n];
        int[] c2 = new int[n];
        Arrays.fill(c1, -1);
        Arrays.fill(c2, -1);

        boolean copyMom = true;
        for (int i = 0; i < n; i++) {
            if (c1[i] != -1) continue;

            int cur = i;
            while (c1[cur] == -1) {
                c1[cur] = copyMom ? mom[cur] : dad[cur];
                c2[cur] = copyMom ? dad[cur] : mom[cur];
                cur = posInMom[dad[cur]];
            }
            copyMom = !copyMom;
        }
        return new int[][] { c1, c2 };
    }

    // ─── Mutation ──────────────────────────────────────────────────────────────

    // Inversion (2-opt): reverse a random sub-sequence.
    private void inversionMutation(int[] c) {
        int l = rng.nextInt(n);
        int r = rng.nextInt(n);
        if (l == r) return;
        if (l > r) { int t = l; l = r; r = t; }
        for (int i = l, j = r; i < j; i++, j--) {
            int t = c[i]; c[i] = c[j]; c[j] = t;
        }
    }

    // Insert: remove one city and re-insert it at another position.
    private void insertMutation(int[] c) {
        int from = rng.nextInt(n);
        int to   = rng.nextInt(n);
        if (from == to) return;
        int city = c[from];
        if (from < to) for (int i = from; i < to; i++) c[i] = c[i + 1];
        else           for (int i = from; i > to; i--) c[i] = c[i - 1];
        c[to] = city;
    }

    // Shuffle: randomise a random sub-sequence.
    private void shuffleMutation(int[] c) {
        int l = rng.nextInt(n);
        int r = rng.nextInt(n);
        if (l > r) { int t = l; l = r; r = t; }
        for (int i = r; i > l; i--) {
            int j = l + rng.nextInt(i - l + 1);
            int t = c[i]; c[i] = c[j]; c[j] = t;
        }
    }

    private void mutate(int[] c) {
        switch (rng.nextInt(6)) {
            case 0: case 1: inversionMutation(c); break;
            case 2: case 3: insertMutation(c);    break;
            case 4: case 5: shuffleMutation(c);   break;
        }
    }

    // ─── Selection ─────────────────────────────────────────────────────────────

    // Stochastic Universal Sampling — inverted for minimisation.
    // Shorter tours get higher weight: inv[i] = maxFit - fit[i] + 1.
    private int[] stochasticUniversalSampling() {
        int sampleSize = Math.max(2, (int)(p * SURVIVE_PCT));

        double maxFit = fit[p - 1];  // pop is sorted ascending, so last = worst
        double total  = 0.0;
        double[] inv  = new double[p];
        for (int i = 0; i < p; i++) {
            inv[i] = maxFit - fit[i] + 1.0;
            total += inv[i];
        }

        double[] cum = new double[p];
        cum[0] = inv[0] / total;
        for (int i = 1; i < p; i++) cum[i] = cum[i - 1] + inv[i] / total;
        cum[p - 1] = 1.0;  // clamp

        double step  = 1.0 / sampleSize;
        double start = rng.nextDouble() * step;

        int[] selected = new int[sampleSize];
        int j = 0;
        for (int k = 0; k < sampleSize; k++) {
            double point = start + k * step;
            while (j < p - 1 && cum[j] < point) j++;
            selected[k] = j;
        }
        return selected;
    }

    // ─── Breeding ──────────────────────────────────────────────────────────────

    private List<int[]> breed(int[] selected) {
        List<int[]> offspring = new ArrayList<>(newGenSize);
        int s = selected.length;

        while (offspring.size() < newGenSize) {
            int[] mom = pop[selected[rng.nextInt(s)]];
            int[] dad = pop[selected[rng.nextInt(s)]];

            int[][] children;
            switch (rng.nextInt(6)) {
                case 0: case 1: case 2: children = orderCrossover(mom, dad);    break;
                case 3: case 4:         children = twoPointCrossover(mom, dad); break;
                default:                children = cyclicCrossover(mom, dad);   break;
            }

            mutate(children[0]);
            mutate(children[1]);
            offspring.add(children[0]);
            offspring.add(children[1]);
        }
        return offspring;
    }

    // ─── Survivor selection ────────────────────────────────────────────────────

    private void updatePopulation(List<int[]> offspring) {
        int eliteCount = Math.max(1, (int)(p * ELITISM_PCT));
        int nonElite   = p - eliteCount;
        int poolSize   = nonElite + offspring.size();

        int[][]  pool    = new int[poolSize][];
        double[] poolFit = new double[poolSize];

        for (int i = 0; i < nonElite; i++) {
            pool[i]    = pop[eliteCount + i];
            poolFit[i] = fit[eliteCount + i];
        }
        for (int i = 0; i < offspring.size(); i++) {
            pool[nonElite + i]    = offspring.get(i);
            poolFit[nonElite + i] = tourLength(offspring.get(i));
        }

        // Keep the best `nonElite` from the pool
        Integer[] order = new Integer[poolSize];
        for (int i = 0; i < poolSize; i++) order[i] = i;
        Arrays.sort(order, (a, b) -> Double.compare(poolFit[a], poolFit[b]));

        boolean[] kept = new boolean[poolSize];
        for (int i = 0; i < nonElite; i++) {
            pop[eliteCount + i] = pool[order[i]];
            fit[eliteCount + i] = poolFit[order[i]];
            kept[order[i]] = true;
        }
        // Discarded chromosomes become eligible for GC (no explicit free needed in Java)

        sortByFitness();
    }

    // ─── Immigrant injection ───────────────────────────────────────────────────

    // Replace the worst IMMIGRANT_PCT individuals with fresh random tours.
    private void injectImmigrants() {
        int count = Math.max(1, (int)(p * IMMIGRANT_PCT));
        int[] perm = new int[n];
        for (int i = 0; i < n; i++) perm[i] = i;
        for (int i = p - count; i < p; i++) {
            shuffleArray(perm);
            pop[i] = Arrays.copyOf(perm, n);
            fit[i] = tourLength(pop[i]);
        }
        sortByFitness();
    }

    // ─── Main GA loop ──────────────────────────────────────────────────────────

    private void evolve() {
        int stagnation  = 0;
        int generations = 0;
        double prevBest = fit[0];

        printBest(generations);  // baseline: initial population

        while (generations < MAX_GEN && stagnation < STAGNATION_GEN) {
            generations++;

            int[]       parents  = stochasticUniversalSampling();
            List<int[]> children = breed(parents);
            updatePopulation(children);

            if (generations % IMMIGRANT_INTERVAL == 0)
                injectImmigrants();

            if (generations == 1 || generations % PRINT_EVERY == 0)
                printBest(generations);

            if (prevBest - fit[0] >= STAGNATION_EPS) {
                stagnation = 0;
                prevBest   = fit[0];
            } else {
                stagnation++;
            }
        }

        System.out.printf("Gen %4d: %sbest = %.2f%s  (final)%n",
                          generations, GRN, fit[0], RST);
        System.out.printf("%nTotal generations: %d%n", generations);
        System.out.printf("Best tour length: %.4f%n", fit[0]);

        if (useUK12) {
            System.out.print("Best tour: ");
            for (int i = 0; i < n; i++)
                System.out.print(cityNames[pop[0][i]] + (i < n - 1 ? " → " : "\n"));
            System.out.println("Known optimal (these coords): ~1872.78");
        } else {
            System.out.print("Best tour: ");
            for (int i = 0; i < n; i++)
                System.out.print(pop[0][i] + (i < n - 1 ? " → " : "\n"));
        }
    }

    private void printBest(int gen) {
        System.out.printf("Gen %4d: %sbest = %.2f%s%n", gen, GRN, fit[0], RST);
    }

    // ─── Utility ───────────────────────────────────────────────────────────────

    private void shuffleArray(int[] a) {
        for (int i = a.length - 1; i > 0; i--) {
            int j = rng.nextInt(i + 1);
            int t = a[i]; a[i] = a[j]; a[j] = t;
        }
    }

    // ─── Entry point ───────────────────────────────────────────────────────────

    public static void main(String[] args) {
        System.out.println(BLU + "=== Travelling Salesman Problem — Genetic Algorithm ===" + RST);
        System.out.println();
        System.out.print("Mode: (1) random cities  (2) UK-12 benchmark\nChoice: ");

        Scanner sc = new Scanner(System.in);
        int mode = sc.nextInt();

        double[] cx, cy;
        String[] cityNames = null;
        int      n;
        boolean  useUK12 = (mode == 2);

        if (useUK12) {
            // Load city names from CSV
            List<String> names = new ArrayList<>();
            try (BufferedReader br = new BufferedReader(new FileReader(DATA_DIR + "uk12_name.csv"))) {
                String line;
                while ((line = br.readLine()) != null)
                    if (!line.isBlank()) names.add(line.trim());
            } catch (IOException e) {
                System.err.println("Cannot open " + DATA_DIR + "uk12_name.csv: " + e.getMessage());
                return;
            }
            // Load coordinates from CSV
            List<Double> xs = new ArrayList<>(), ys = new ArrayList<>();
            try (BufferedReader br = new BufferedReader(new FileReader(DATA_DIR + "uk12_xy.csv"))) {
                String line;
                while ((line = br.readLine()) != null) {
                    if (line.isBlank()) continue;
                    String[] parts = line.split(",");
                    xs.add(Double.parseDouble(parts[0].trim()));
                    ys.add(Double.parseDouble(parts[1].trim()));
                }
            } catch (IOException e) {
                System.err.println("Cannot open " + DATA_DIR + "uk12_xy.csv: " + e.getMessage());
                return;
            }
            n         = names.size();
            cx        = xs.stream().mapToDouble(Double::doubleValue).toArray();
            cy        = ys.stream().mapToDouble(Double::doubleValue).toArray();
            cityNames = names.toArray(new String[0]);
        } else {
            System.out.print(BLU + "Enter N (number of cities, N ≤ 100): " + RST);
            n  = sc.nextInt();
            cx = new double[n];
            cy = new double[n];
            Random gen = new Random(42);
            for (int i = 0; i < n; i++) {
                cx[i] = COORD_LO + gen.nextDouble() * (COORD_HI - COORD_LO);
                cy[i] = COORD_LO + gen.nextDouble() * (COORD_HI - COORD_LO);
            }
        }

        System.out.print(BLU + "Enter P (population size, e.g. 150): " + RST);
        int p = sc.nextInt();

        TspGa ga = new TspGa(n, cx, cy, cityNames, p, useUK12, 42L);
        ga.evolve();
    }
}
