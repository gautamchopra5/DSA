#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <unordered_map>

using namespace std;

// ---------- DSU (Disjoint Set Union) ----------
struct DSU {
    vector<int> parent;
    vector<int> sz;

    DSU(int n) : parent(n), sz(n, 1) {
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] == x) return x;
        return parent[x] = find(parent[x]);
    }

    // returns true if merge actually happened
    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        parent[b] = a;
        sz[a] += sz[b];
        return true;
    }

    int size(int x) {
        return sz[find(x)];
    }
};

struct Point {
    long long x, y, z;
};

struct Edge {
    long long d2;
    int u, v;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // --- Read points from file ---
    ifstream infile("inputs/inputs08.txt");
    if (!infile) {
        cerr << "Failed to open inputs/inputs08.txt\n";
        return 1;
    }

    vector<Point> pts;
    string line;
    while (getline(infile, line)) {
        if (line.empty()) continue;
        line.erase(remove(line.begin(), line.end(), ' '), line.end());
        if (line.empty()) continue;

        long long x, y, z;
        char c1, c2;
        stringstream ss(line);
        if (!(ss >> x >> c1 >> y >> c2 >> z)) continue;
        pts.push_back({x, y, z});
    }

    int n = pts.size();
    if (n == 0) {
        cerr << "Empty input.\n";
        return 1;
    }

    // --- Generate all edges ---
    vector<Edge> edges;
    edges.reserve((long long)n * (n - 1) / 2);

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            long long dx = pts[i].x - pts[j].x;
            long long dy = pts[i].y - pts[j].y;
            long long dz = pts[i].z - pts[j].z;
            long long d2 = dx*dx + dy*dy + dz*dz;
            edges.push_back({d2, i, j});
        }
    }

    // --- Sort edges by distance ---
    sort(edges.begin(), edges.end(),
         [](const Edge &a, const Edge &b) {
             return a.d2 < b.d2;
         });

    // ---------------------------
    // PART 1: First 1000 merges
    // ---------------------------
    const int K = 1000;

    DSU dsu1(n);

    int limit = min((int)edges.size(), K);
    for (int i = 0; i < limit; ++i) {
        dsu1.unite(edges[i].u, edges[i].v);
    }

    // Count component sizes
    unordered_map<int, long long> compSize;
    compSize.reserve(n * 2);

    for (int i = 0; i < n; ++i) {
        int r = dsu1.find(i);
        compSize[r]++;
    }

    vector<long long> sizes;
    sizes.reserve(compSize.size());
    for (auto &p : compSize) {
        sizes.push_back(p.second);
    }

    sort(sizes.begin(), sizes.end(), greater<long long>());
    long long part1 = sizes[0] * sizes[1] * sizes[2];


    // ---------------------------
    // PART 2: Merge until single component
    // ---------------------------
    DSU dsu2(n);
    int components = n;

    long long part2 = -1;

    for (auto &e : edges) {
        if (dsu2.unite(e.u, e.v)) {
            components--;
            if (components == 1) {
                // All merged now -> record x[u] * x[v]
                long long xprod = pts[e.u].x * pts[e.v].x;
                part2 = xprod;
                break;
            }
        }
    }

    // Print answers
    cout << "Part 1: " << part1 << "\n";
    cout << "Part 2: " << part2 << "\n";

    return 0;
}
