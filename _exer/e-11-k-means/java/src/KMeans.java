import java.io.*;
import java.util.*;

public class KMeans {

    static final double CENTROID_MOVE_EPSILON = 1e-9;
    static final int    RANDOM_RESTARTS       = 100;
    static final int    MAX_ITERATIONS        = 300;

    // ─── Point ────────────────────────────────────────────────────────────────

    record Point(double x, double y) {}

    static double sqDist(Point a, Point b) {
        double dx = a.x() - b.x(), dy = a.y() - b.y();
        return dx * dx + dy * dy;
    }

    // ─── I/O ──────────────────────────────────────────────────────────────────

    static List<Point> readPoints(String path) throws IOException {
        List<Point> pts = new ArrayList<>();
        try (BufferedReader br = new BufferedReader(new FileReader(path))) {
            String line;
            while ((line = br.readLine()) != null) {
                line = line.trim();
                if (line.isEmpty()) continue;
                String[] parts = line.split("\\s+");
                pts.add(new Point(Double.parseDouble(parts[0]), Double.parseDouble(parts[1])));
            }
        }
        return pts;
    }

    static void writeResult(String path, List<Point> pts, int[] labels, List<Point> centroids)
            throws IOException {
        try (PrintWriter pw = new PrintWriter(new FileWriter(path))) {
            pw.println("type,x,y,cluster");
            for (int i = 0; i < pts.size(); i++)
                pw.printf("point,%s,%s,%d%n", pts.get(i).x(), pts.get(i).y(), labels[i]);
            for (int k = 0; k < centroids.size(); k++)
                pw.printf("centroid,%s,%s,%d%n", centroids.get(k).x(), centroids.get(k).y(), k);
        }
    }

    // ─── Initialisations ──────────────────────────────────────────────────────

    static List<Point> initRandom(List<Point> pts, int K, Random rng) {
        double minX = pts.stream().mapToDouble(Point::x).min().orElse(0);
        double maxX = pts.stream().mapToDouble(Point::x).max().orElse(1);
        double minY = pts.stream().mapToDouble(Point::y).min().orElse(0);
        double maxY = pts.stream().mapToDouble(Point::y).max().orElse(1);
        List<Point> c = new ArrayList<>();
        for (int k = 0; k < K; k++)
            c.add(new Point(minX + rng.nextDouble() * (maxX - minX),
                            minY + rng.nextDouble() * (maxY - minY)));
        return c;
    }

    static List<Point> initPlusPlus(List<Point> pts, int K, Random rng) {
        int n = pts.size();
        List<Point> c = new ArrayList<>();
        c.add(pts.get(rng.nextInt(n)));
        double[] d = new double[n];
        Arrays.fill(d, Double.MAX_VALUE);

        for (int k = 1; k < K; k++) {
            for (int i = 0; i < n; i++) {
                double dist = sqDist(pts.get(i), c.get(c.size() - 1));
                if (dist < d[i]) d[i] = dist;
            }
            double total = Arrays.stream(d).sum();
            double r = rng.nextDouble() * total;
            double cumul = 0;
            int chosen = n - 1;
            for (int i = 0; i < n; i++) {
                cumul += d[i];
                if (cumul >= r) { chosen = i; break; }
            }
            c.add(pts.get(chosen));
        }
        return c;
    }

    // ─── K-Means core ─────────────────────────────────────────────────────────

    record RunResult(int[] labels, List<Point> centroids) {}

    static RunResult runKMeans(List<Point> pts, List<Point> init) {
        int n = pts.size();
        int K = init.size();
        List<Point> centroids = new ArrayList<>(init);
        int[] labels = new int[n];

        for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
            // Assignment
            for (int i = 0; i < n; i++) {
                double best = Double.MAX_VALUE;
                for (int k = 0; k < K; k++) {
                    double d = sqDist(pts.get(i), centroids.get(k));
                    if (d < best) { best = d; labels[i] = k; }
                }
            }

            // Update
            double[] sx = new double[K], sy = new double[K];
            int[] cnt = new int[K];
            for (int i = 0; i < n; i++) {
                sx[labels[i]] += pts.get(i).x();
                sy[labels[i]] += pts.get(i).y();
                cnt[labels[i]]++;
            }

            boolean moved = false;
            for (int k = 0; k < K; k++) {
                if (cnt[k] == 0) continue;
                Point nc = new Point(sx[k] / cnt[k], sy[k] / cnt[k]);
                if (sqDist(nc, centroids.get(k)) > CENTROID_MOVE_EPSILON) {
                    centroids.set(k, nc);
                    moved = true;
                }
            }
            if (!moved) break;
        }

        return new RunResult(labels, centroids);
    }

    // ─── Quality metrics ──────────────────────────────────────────────────────

    static double wcss(List<Point> pts, int[] labels, List<Point> centroids) {
        double sum = 0;
        for (int i = 0; i < pts.size(); i++)
            sum += sqDist(pts.get(i), centroids.get(labels[i]));
        return sum;
    }

    static double minInterCentroid(List<Point> centroids) {
        double minD = Double.MAX_VALUE;
        for (int i = 0; i < centroids.size(); i++)
            for (int j = i + 1; j < centroids.size(); j++)
                minD = Math.min(minD, sqDist(centroids.get(i), centroids.get(j)));
        return minD;
    }

    static double maxIntraClusterDist(List<Point> pts, int[] labels, List<Point> centroids) {
        double[] maxR = new double[centroids.size()];
        for (int i = 0; i < pts.size(); i++) {
            int k = labels[i];
            double d = sqDist(pts.get(i), centroids.get(k));
            if (d > maxR[k]) maxR[k] = d;
        }
        return Arrays.stream(maxR).max().orElse(0) * 4.0; // (2r)^2 = 4r^2
    }

    static double dunnIndex(List<Point> pts, int[] labels, List<Point> centroids) {
        double inter = minInterCentroid(centroids);
        double intra = maxIntraClusterDist(pts, labels, centroids);
        if (intra < 1e-15) return Double.MAX_VALUE;
        return inter / intra;
    }

    // ─── Random Restart ───────────────────────────────────────────────────────

    enum Metric { INNER, OUTER, BOTH }

    static RunResult randomRestart(List<Point> pts, int K, Metric metric,
                                   boolean plusPlus, long seed) {
        Random rng = new Random(seed);
        RunResult best = null;
        double bestScore = (metric == Metric.INNER) ? Double.MAX_VALUE : -Double.MAX_VALUE;

        for (int r = 0; r < RANDOM_RESTARTS; r++) {
            List<Point> c0 = plusPlus ? initPlusPlus(pts, K, rng) : initRandom(pts, K, rng);
            RunResult res = runKMeans(pts, c0);

            double score = switch (metric) {
                case INNER -> wcss(pts, res.labels(), res.centroids());
                case OUTER -> minInterCentroid(res.centroids());
                case BOTH  -> dunnIndex(pts, res.labels(), res.centroids());
            };

            boolean better = (metric == Metric.INNER) ? score < bestScore : score > bestScore;
            if (better) { bestScore = score; best = res; }
        }

        String metricName = switch (metric) {
            case INNER -> "WCSS (inner)";
            case OUTER -> "min-inter-centroid";
            case BOTH  -> "Dunn index";
        };
        String initName = plusPlus ? "K-Means++" : "K-Means";
        System.out.printf("[%s | %s] best score = %.4f%n", initName, metricName, bestScore);

        return best;
    }

    // ─── Main ─────────────────────────────────────────────────────────────────

    public static void main(String[] args) throws Exception {
        String dataFile;
        int K;
        Metric metric;
        boolean plusPlus;

        if (args.length >= 2) {
            dataFile = args[0];
            K        = Integer.parseInt(args[1]);
            metric   = Metric.BOTH;
            plusPlus = true;
        } else {
            Scanner sc = new Scanner(System.in);
            System.out.print("\033[36;1mK-Means Clustering\033[0m\n\nData file: ");
            dataFile = sc.next();
            System.out.print("Number of clusters K: ");
            K = sc.nextInt();
            System.out.print("Metric [1=WCSS, 2=inter-centroid, 3=Dunn]: ");
            int m = sc.nextInt();
            metric = (m == 1) ? Metric.INNER : (m == 2) ? Metric.OUTER : Metric.BOTH;
            System.out.print("Init [1=random, 2=K-Means++]: ");
            plusPlus = sc.nextInt() == 2;
        }

        List<Point> pts = readPoints(dataFile);
        if (pts.isEmpty()) throw new IllegalArgumentException("Empty dataset");
        System.out.printf("Loaded %d points, K=%d%n", pts.size(), K);

        RunResult res = randomRestart(pts, K, metric, plusPlus, 42L);

        String outCsv = (args.length >= 3) ? args[2] :
                new File(KMeans.class.getProtectionDomain()
                        .getCodeSource().getLocation().toURI().getPath())
                        .getParentFile().getPath() + "/result.csv";
        writeResult(outCsv, pts, res.labels(), res.centroids());
        System.out.println("Result written to " + outCsv);
    }
}
