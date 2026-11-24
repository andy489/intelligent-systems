#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <random>
#include <cmath>
#include <cstdio>
#include <climits>
#include <libgen.h>

static const int NUM_ATTRS = 16;
static const int K = 3; // y, n, ?

int valueIndex(const std::string& v) {
    if (v == "y") return 0;
    if (v == "n") return 1;
    return 2;
}

int main(int argc, char* argv[]) {
    // resolve data path relative to the binary's directory
    char buf[PATH_MAX];
    realpath(argv[0], buf);
    std::string binDir = dirname(buf);
    std::string dataFile = binDir + "/../data/house-votes-84.data";

    std::vector<std::vector<std::string>> data;
    std::ifstream fin(dataFile);
    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        std::vector<std::string> row;
        std::istringstream ss(line);
        std::string tok;
        while (std::getline(ss, tok, ',')) row.push_back(tok);
        if ((int)row.size() == NUM_ATTRS + 1) data.push_back(row);
    }
    fin.close();

    std::mt19937 rng(42);
    std::shuffle(data.begin(), data.end(), rng);

    int n = (int)data.size();
    int numFolds = 10;
    std::vector<int> foldSizes(numFolds);
    int base = n / numFolds, extra = n % numFolds;
    for (int i = 0; i < numFolds; i++) foldSizes[i] = base + (i < extra ? 1 : 0);

    double totalAcc = 0.0;
    int start = 0;
    for (int fold = 0; fold < numFolds; fold++) {
        int end = start + foldSizes[fold];

        // counts[cls][attr][val]
        std::map<std::string, std::vector<std::vector<int>>> counts;
        std::map<std::string, int> classCount;

        for (int i = 0; i < n; i++) {
            if (i >= start && i < end) continue; // skip test fold
            const auto& row = data[i];
            const std::string& cls = row[0];
            classCount[cls]++;
            if (counts.find(cls) == counts.end())
                counts[cls] = std::vector<std::vector<int>>(NUM_ATTRS, std::vector<int>(K, 0));
            for (int a = 0; a < NUM_ATTRS; a++)
                counts[cls][a][valueIndex(row[a + 1])]++;
        }

        std::vector<std::string> classes;
        for (auto& kv : classCount) classes.push_back(kv.first);
        int totalTrain = n - (end - start);

        int correct = 0;
        for (int i = start; i < end; i++) {
            const auto& row = data[i];
            const std::string& trueClass = row[0];
            std::string bestCls;
            double bestScore = -1e300;
            for (const auto& cls : classes) {
                int cc = classCount[cls];
                double score = std::log((double)cc / totalTrain);
                for (int a = 0; a < NUM_ATTRS; a++) {
                    int vi = valueIndex(row[a + 1]);
                    double p = (counts[cls][a][vi] + 1.0) / (cc + K);
                    score += std::log(p);
                }
                if (score > bestScore) { bestScore = score; bestCls = cls; }
            }
            if (bestCls == trueClass) correct++;
        }

        int foldSize = end - start;
        double acc = (double)correct / foldSize;
        totalAcc += acc;
        printf("Fold %2d: accuracy = %.4f (%d/%d correct)\n", fold + 1, acc, correct, foldSize);
        start = end;
    }
    printf("Mean accuracy: %.4f\n", totalAcc / numFolds);
    return 0;
}
