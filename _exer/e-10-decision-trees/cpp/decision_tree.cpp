#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <algorithm>
#include <memory>

using namespace std;

// ─── Data ─────────────────────────────────────────────────────────────────────

struct Dataset {
    vector<string>              headers;   // attribute names (last = class)
    vector<vector<string>>      rows;
    int numAttrs() const { return (int)headers.size() - 1; }
    string classOf(const vector<string>& row) const { return row.back(); }
};

static Dataset readCSV(const string& path) {
    Dataset ds;
    ifstream in(path);
    if (!in) { cerr << "Cannot open: " << path << '\n'; exit(1); }
    string line;
    bool first = true;
    while (getline(in, line)) {
        if (line.empty()) continue;
        istringstream ss(line);
        string tok;
        vector<string> row;
        while (getline(ss, tok, ',')) row.push_back(tok);
        if (first) { ds.headers = row; first = false; }
        else        ds.rows.push_back(row);
    }
    return ds;
}

// ─── Entropy / Gain ───────────────────────────────────────────────────────────

static double entropy(const vector<vector<string>>& rows, int classCol) {
    map<string, int> cnt;
    for (auto& r : rows) cnt[r[classCol]]++;
    double n = (double)rows.size(), h = 0;
    for (auto& [v, c] : cnt) {
        double p = c / n;
        h -= p * log2(p);
    }
    return h;
}

static double gain(const vector<vector<string>>& rows, int attr, int classCol) {
    double base = entropy(rows, classCol);
    map<string, vector<vector<string>>> parts;
    for (auto& r : rows) parts[r[attr]].push_back(r);
    double n = (double)rows.size(), rem = 0;
    for (auto& [v, sub] : parts)
        rem += (sub.size() / n) * entropy(sub, classCol);
    return base - rem;
}

// ─── ID3 Node ─────────────────────────────────────────────────────────────────

struct Node {
    bool   isLeaf = false;
    string label;          // leaf: class label
    int    attrIdx = -1;   // internal: attribute index
    string attrName;
    map<string, unique_ptr<Node>> children;
};

static string majority(const vector<vector<string>>& rows, int classCol) {
    map<string, int> cnt;
    for (auto& r : rows) cnt[r[classCol]]++;
    return max_element(cnt.begin(), cnt.end(),
        [](auto& a, auto& b){ return a.second < b.second; })->first;
}

static unique_ptr<Node> build(const vector<vector<string>>& rows,
                               const vector<int>& attrs,
                               int classCol,
                               const string& defaultClass) {
    auto node = make_unique<Node>();

    if (rows.empty()) {
        node->isLeaf = true; node->label = defaultClass; return node;
    }

    // All same class?
    string first = rows[0][classCol];
    if (all_of(rows.begin(), rows.end(),
               [&](auto& r){ return r[classCol] == first; })) {
        node->isLeaf = true; node->label = first; return node;
    }

    if (attrs.empty()) {
        node->isLeaf = true; node->label = majority(rows, classCol); return node;
    }

    // Best attribute
    int bestAttr = attrs[0];
    double bestGain = -1;
    for (int a : attrs) {
        double g = gain(rows, a, classCol);
        if (g > bestGain) { bestGain = g; bestAttr = a; }
    }

    node->attrIdx  = bestAttr;
    node->attrName = rows[0].size() > 0 ? "" : ""; // filled below

    string maj = majority(rows, classCol);
    vector<int> remaining;
    for (int a : attrs) if (a != bestAttr) remaining.push_back(a);

    map<string, vector<vector<string>>> parts;
    for (auto& r : rows) parts[r[bestAttr]].push_back(r);

    for (auto& [val, sub] : parts)
        node->children[val] = build(sub, remaining, classCol, maj);

    return node;
}

// ─── Print tree ───────────────────────────────────────────────────────────────

static void printTree(const Node* node, const vector<string>& headers,
                      const string& prefix, const string& branch) {
    if (!branch.empty()) {
        // internal call: branch label already printed by parent; just recurse
    }
    if (node->isLeaf) return; // leaf printed inline by parent

    for (auto& [val, child] : node->children) {
        string indent = prefix;
        if (child->isLeaf) {
            cout << indent << headers[node->attrIdx] << " = " << val
                 << " -> " << child->label << '\n';
        } else {
            cout << indent << headers[node->attrIdx] << " = " << val << '\n';
            printTree(child.get(), headers, prefix + "  ", val);
        }
    }
}

// ─── Classify ─────────────────────────────────────────────────────────────────

static string classify(const Node* node, const vector<string>& row) {
    if (node->isLeaf) return node->label;
    const string& val = row[node->attrIdx];
    auto it = node->children.find(val);
    if (it == node->children.end()) {
        // unknown value: return majority of known children (first leaf found)
        return node->children.begin()->second->label;
    }
    return classify(it->second.get(), row);
}

// ─── Main ─────────────────────────────────────────────────────────────────────

int main(int argc, char* argv[]) {
    string csvPath = (argc >= 2) ? argv[1] : "data/play_tennis.csv";
    string query   = (argc >= 3) ? argv[2] : "";

    Dataset ds = readCSV(csvPath);
    int classCol = (int)ds.headers.size() - 1;

    vector<int> attrs;
    for (int i = 0; i < ds.numAttrs(); ++i) attrs.push_back(i);

    auto root = build(ds.rows, attrs, classCol, "");

    // Non-interactive single-query mode
    if (!query.empty()) {
        istringstream ss(query);
        string tok;
        vector<string> row;
        while (getline(ss, tok, ',')) row.push_back(tok);
        cout << classify(root.get(), row) << '\n';
        return 0;
    }

    // Print tree
    cout << "Decision Tree:\n";
    printTree(root.get(), ds.headers, "", "");

    // Training accuracy
    int correct = 0;
    for (auto& r : ds.rows)
        if (classify(root.get(), r) == ds.classOf(r)) ++correct;
    int n = (int)ds.rows.size();
    cout << "\nTraining accuracy: " << correct << '/' << n
         << " (" << fixed;
    cout.precision(2);
    cout << 100.0 * correct / n << "%)\n";

    // Interactive
    vector<string> attrNames(ds.headers.begin(), ds.headers.begin() + classCol);
    cout << "\nEnter attributes (";
    for (int i = 0; i < (int)attrNames.size(); ++i)
        cout << attrNames[i] << (i + 1 < (int)attrNames.size() ? " " : "");
    cout << ") or 'quit':\n";

    string line;
    while (true) {
        cout << "> ";
        if (!getline(cin, line)) break;
        if (line == "quit") break;
        istringstream ss(line);
        vector<string> row;
        string tok;
        while (ss >> tok) row.push_back(tok);
        if ((int)row.size() != (int)attrNames.size()) {
            cout << "Expected " << attrNames.size() << " values.\n";
            continue;
        }
        cout << "Prediction: " << classify(root.get(), row) << '\n';
    }
    return 0;
}
