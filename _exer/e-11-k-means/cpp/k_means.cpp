#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
#include <random>
#include <cassert>
#include <algorithm>
#include <iomanip>

using namespace std;

// ─── Constants ────────────────────────────────────────────────────────────────

static constexpr double CENTROID_MOVE_EPSILON = 1e-9;
static constexpr int    RANDOM_RESTARTS       = 100;
static constexpr int    MAX_ITERATIONS        = 300;

// ─── Point ────────────────────────────────────────────────────────────────────

struct Point {
    double x, y;
};

static inline double sqDist(const Point& a, const Point& b) {
    double dx = a.x - b.x, dy = a.y - b.y;
    return dx * dx + dy * dy;
}

// ─── I/O ──────────────────────────────────────────────────────────────────────

static vector<Point> readPoints(const string& path) {
    ifstream in(path);
    if (!in) { cerr << "Cannot open: " << path << '\n'; exit(1); }
    vector<Point> pts;
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        istringstream ss(line);
        Point p;
        if (ss >> p.x >> p.y) pts.push_back(p);
    }
    return pts;
}

// Write label assignments + centroid markers so Python can plot
static void writeResult(const string& outPath,
                        const vector<Point>& pts,
                        const vector<int>&   labels,
                        const vector<Point>& centroids) {
    ofstream out(outPath);
    // header
    out << "type,x,y,cluster\n";
    for (size_t i = 0; i < pts.size(); ++i)
        out << "point," << pts[i].x << ',' << pts[i].y << ',' << labels[i] << '\n';
    for (size_t i = 0; i < centroids.size(); ++i)
        out << "centroid," << centroids[i].x << ',' << centroids[i].y << ',' << i << '\n';
}

// ─── K-Means core ─────────────────────────────────────────────────────────────

static vector<Point> initRandom(const vector<Point>& pts, int K, mt19937& rng) {
    uniform_real_distribution<double> rx(
        (*min_element(pts.begin(), pts.end(), [](auto& a, auto& b){ return a.x < b.x; })).x,
        (*max_element(pts.begin(), pts.end(), [](auto& a, auto& b){ return a.x < b.x; })).x);
    uniform_real_distribution<double> ry(
        (*min_element(pts.begin(), pts.end(), [](auto& a, auto& b){ return a.y < b.y; })).y,
        (*max_element(pts.begin(), pts.end(), [](auto& a, auto& b){ return a.y < b.y; })).y);
    vector<Point> c(K);
    for (auto& p : c) p = {rx(rng), ry(rng)};
    return c;
}

// K-Means++ initialisation — O(n·K) with guaranteed spread
static vector<Point> initPlusPlus(const vector<Point>& pts, int K, mt19937& rng) {
    const int n = (int)pts.size();
    vector<Point> c;
    c.reserve(K);

    uniform_int_distribution<int> uni(0, n - 1);
    c.push_back(pts[uni(rng)]);

    vector<double> d(n, numeric_limits<double>::max());

    for (int k = 1; k < K; ++k) {
        // update distances from each point to nearest existing centroid
        for (int i = 0; i < n; ++i) {
            double dist = sqDist(pts[i], c.back());
            if (dist < d[i]) d[i] = dist;
        }
        // choose next centroid proportional to D²
        discrete_distribution<int> prop(d.begin(), d.end());
        c.push_back(pts[prop(rng)]);
    }
    return c;
}

struct RunResult {
    vector<int>   labels;
    vector<Point> centroids;
};

// One k-means run from given initial centroids
static RunResult runKMeans(const vector<Point>& pts,
                           vector<Point>        centroids) {
    const int n = (int)pts.size();
    const int K = (int)centroids.size();
    vector<int> labels(n, 0);

    for (int iter = 0; iter < MAX_ITERATIONS; ++iter) {
        // Assignment
        for (int i = 0; i < n; ++i) {
            double best = numeric_limits<double>::max();
            for (int k = 0; k < K; ++k) {
                double d = sqDist(pts[i], centroids[k]);
                if (d < best) { best = d; labels[i] = k; }
            }
        }

        // Update
        vector<double> sx(K, 0), sy(K, 0);
        vector<int>    cnt(K, 0);
        for (int i = 0; i < n; ++i) {
            sx[labels[i]] += pts[i].x;
            sy[labels[i]] += pts[i].y;
            ++cnt[labels[i]];
        }

        bool moved = false;
        for (int k = 0; k < K; ++k) {
            if (cnt[k] == 0) continue;
            Point nc = {sx[k] / cnt[k], sy[k] / cnt[k]};
            if (sqDist(nc, centroids[k]) > CENTROID_MOVE_EPSILON) {
                centroids[k] = nc;
                moved = true;
            }
        }
        if (!moved) break;
    }

    return {labels, centroids};
}

// ─── Quality metrics ──────────────────────────────────────────────────────────

// Within-Cluster Sum of Squares (WCSS) — minimise
static double wcss(const vector<Point>& pts,
                   const vector<int>&   labels,
                   const vector<Point>& centroids) {
    double sum = 0;
    for (size_t i = 0; i < pts.size(); ++i)
        sum += sqDist(pts[i], centroids[labels[i]]);
    return sum;
}

// Min inter-centroid squared distance — maximise
static double minInterCentroid(const vector<Point>& centroids) {
    double minD = numeric_limits<double>::max();
    for (size_t i = 0; i < centroids.size(); ++i)
        for (size_t j = i + 1; j < centroids.size(); ++j)
            minD = min(minD, sqDist(centroids[i], centroids[j]));
    return minD;
}

// Max intra-cluster diameter approximated as 2 × max(point-to-centroid) — O(n·K)
static double maxIntraClusterDist(const vector<Point>& pts,
                                  const vector<int>&   labels,
                                  const vector<Point>& centroids) {
    int K = (int)centroids.size();
    vector<double> maxR(K, 0.0);
    for (size_t i = 0; i < pts.size(); ++i)
        maxR[labels[i]] = max(maxR[labels[i]], sqDist(pts[i], centroids[labels[i]]));
    return *max_element(maxR.begin(), maxR.end()) * 4.0; // (2r)² = 4r²
}

// Dunn index = min inter-centroid distance / max intra-cluster diameter — maximise
// (approximation using centroids for inter, diameter for intra)
static double dunnIndex(const vector<Point>& pts,
                        const vector<int>&   labels,
                        const vector<Point>& centroids) {
    double inter = minInterCentroid(centroids);
    double intra = maxIntraClusterDist(pts, labels, centroids);
    if (intra < 1e-15) return numeric_limits<double>::max();
    return inter / intra;
}

// ─── Random Restart ───────────────────────────────────────────────────────────

enum class Metric  { INNER, OUTER, BOTH };
enum class InitMode{ RANDOM, PLUS_PLUS };

static RunResult randomRestart(const vector<Point>& pts, int K,
                               Metric metric, InitMode init,
                               unsigned seed = 42) {
    mt19937 rng(seed);

    RunResult best;
    double bestScore = (metric == Metric::INNER) ?
                           numeric_limits<double>::max() : -1.0;

    for (int r = 0; r < RANDOM_RESTARTS; ++r) {
        vector<Point> c0 = (init == InitMode::PLUS_PLUS) ?
                               initPlusPlus(pts, K, rng) :
                               initRandom(pts, K, rng);
        RunResult res = runKMeans(pts, c0);

        double score = 0.0;
        switch (metric) {
            case Metric::INNER: score = wcss(pts, res.labels, res.centroids);              break;
            case Metric::OUTER: score = minInterCentroid(res.centroids);                   break;
            case Metric::BOTH:  score = dunnIndex(pts, res.labels, res.centroids);         break;
        }

        bool better = (metric == Metric::INNER) ? (score < bestScore)
                                                 : (score > bestScore);
        if (better) {
            bestScore = score;
            best = res;
        }
    }

    // Print scores
    const char* metricName = (metric == Metric::INNER) ? "WCSS (inner)" :
                             (metric == Metric::OUTER) ? "min-inter-centroid" :
                                                          "Dunn index";
    const char* initName = (init == InitMode::PLUS_PLUS) ? "K-Means++" : "K-Means";
    cout << fixed << setprecision(4)
         << "[" << initName << " | " << metricName << "] "
         << "best score = " << bestScore << '\n';

    return best;
}

// ─── Main ─────────────────────────────────────────────────────────────────────

static int askInt(const string& prompt, int lo, int hi) {
    int v;
    while (true) {
        cout << "\033[33;1m" << prompt << "\033[0m";
        if (cin >> v && v >= lo && v <= hi) return v;
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Enter a value in [" << lo << ", " << hi << "]\n";
    }
}

int main(int argc, char* argv[]) {
    // ── Input ──────────────────────────────────────────────────────────────────
    string dataFile;
    int    K;
    int    metricChoice;
    int    initChoice;

    if (argc >= 3) {
        dataFile     = argv[1];
        K            = stoi(argv[2]);
        metricChoice = 3;
        initChoice   = 2;
    } else {
        cout << "\033[36;1mK-Means Clustering\033[0m\n\n";
        cout << "Data file (e.g. ../Datasets/normal/normal.txt): ";
        cin >> dataFile;
        K = askInt("Number of clusters K: ", 1, 20);
        metricChoice = askInt("Metric [1=WCSS(inner), 2=inter-centroid, 3=Dunn(both)]: ", 1, 3);
        initChoice   = askInt("Init [1=random, 2=K-Means++]: ", 1, 2);
    }

    vector<Point> pts = readPoints(dataFile);
    assert(!pts.empty());
    cout << "Loaded " << pts.size() << " points, K=" << K << '\n';

    Metric  metric = (metricChoice == 1) ? Metric::INNER :
                     (metricChoice == 2) ? Metric::OUTER : Metric::BOTH;
    InitMode init  = (initChoice == 2)   ? InitMode::PLUS_PLUS : InitMode::RANDOM;

    RunResult res = randomRestart(pts, K, metric, init);

    // ── Output ─────────────────────────────────────────────────────────────────
    string outCsv = (argc >= 4) ? argv[3] : "result.csv";
    writeResult(outCsv, pts, res.labels, res.centroids);
    cout << "Result written to " << outCsv << '\n';

    return 0;
}
