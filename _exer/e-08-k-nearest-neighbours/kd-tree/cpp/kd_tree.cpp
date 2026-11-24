#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>
#include <iomanip>

struct Node {
    double x, y;
    int idx;
    Node* left;
    Node* right;
    Node(double x, double y, int idx) : x(x), y(y), idx(idx), left(nullptr), right(nullptr) {}
};

Node* build(std::vector<Node*>& pts, int depth) {
    if (pts.empty()) return nullptr;

    int axis = depth % 2;
    std::sort(pts.begin(), pts.end(), [axis](Node* a, Node* b) {
        return axis == 0 ? a->x < b->x : a->y < b->y;
    });

    int mid = pts.size() / 2;
    Node* node = pts[mid];

    std::vector<Node*> left_pts(pts.begin(), pts.begin() + mid);
    std::vector<Node*> right_pts(pts.begin() + mid + 1, pts.end());

    node->left  = build(left_pts,  depth + 1);
    node->right = build(right_pts, depth + 1);
    return node;
}

void nearest(Node* node, double qx, double qy, int depth,
             double& bestDist, int& bestIdx,
             const std::vector<std::pair<double,double>>& coords) {
    if (!node) return;

    double dx = node->x - qx;
    double dy = node->y - qy;
    double d  = std::sqrt(dx*dx + dy*dy);

    if (d < bestDist || (d == bestDist && node->idx < bestIdx)) {
        bestDist = d;
        bestIdx  = node->idx;
    }

    int axis = depth % 2;
    double diff = (axis == 0 ? qx : qy) - (axis == 0 ? node->x : node->y);

    Node* near = diff <= 0 ? node->left  : node->right;
    Node* far  = diff <= 0 ? node->right : node->left;

    nearest(near, qx, qy, depth + 1, bestDist, bestIdx, coords);

    if (std::abs(diff) < bestDist) {
        nearest(far, qx, qy, depth + 1, bestDist, bestIdx, coords);
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::vector<Node*> pts;
    std::vector<std::pair<double,double>> coords(n);
    for (int i = 0; i < n; i++) {
        double x, y;
        std::cin >> x >> y;
        coords[i] = {x, y};
        pts.push_back(new Node(x, y, i));
    }

    Node* root = build(pts, 0);

    int q;
    std::cin >> q;

    std::cout << std::fixed << std::setprecision(2);

    for (int i = 0; i < q; i++) {
        double qx, qy;
        std::cin >> qx >> qy;

        double bestDist = std::numeric_limits<double>::infinity();
        int    bestIdx  = -1;
        nearest(root, qx, qy, 0, bestDist, bestIdx, coords);

        double px = coords[bestIdx].first;
        double py = coords[bestIdx].second;

        std::cout << "Nearest to (" << qx << ", " << qy << "): point " << bestIdx
                  << " (" << px << ", " << py << ") dist="
                  << std::setprecision(4) << bestDist
                  << std::setprecision(2) << "\n";
    }

    return 0;
}
