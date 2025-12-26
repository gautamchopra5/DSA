#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <cmath>

using namespace std;

using ll = long long;

struct Point {
    ll x, y;
    bool operator==(const Point& other) const noexcept {
        return x == other.x && y == other.y;
    }
};

// Hash for Point so we can use unordered_set<Point>
struct PointHash {
    size_t operator()(const Point& p) const noexcept {
        // Simple 64-bit mix from x,y
        auto h1 = std::hash<ll>()(p.x);
        auto h2 = std::hash<ll>()(p.y);
        // standard combine
        return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
    }
};

static unordered_set<Point, PointHash> boundary;

// Check if line segments AB and CD intersect in the "bad" way
// (not just touching; proper intersection).
static bool doSegmentsIntersect(const Point& A, const Point& B,
                                const Point& C, const Point& D) {
    ll mincxdx = min(C.x, D.x), maxcxdx = max(C.x, D.x);
    ll mincydy = min(C.y, D.y), maxcydy = max(C.y, D.y);
    ll minaxbx = min(A.x, B.x), maxaxbx = max(A.x, B.x);
    ll minayby = min(A.y, B.y), maxayby = max(A.y, B.y);

    bool abxInRange = (minaxbx <= maxcxdx) && (maxaxbx >= mincxdx);
    bool abyInRange = (minayby <= maxcydy) && (maxayby >= mincydy);

    return abxInRange && abyInRange
        // exclude pure touching cases (as per your Java comment)
        && C.x != A.x && C.y != A.y && D.x != B.x && D.y != B.y;
}

// Ray-casting point-in-polygon with "on boundary" handled via the boundary set
static bool pointOnOrInPolygon(const Point& point, const vector<Point>& polygon) {
    if (boundary.find(point) != boundary.end()) {
        return true;
    }

    int numVertices = (int)polygon.size();
    double x = (double)point.x;
    double y = (double)point.y;
    bool inside = false;

    Point p1 = polygon[0];
    for (int i = 1; i <= numVertices; ++i) {
        Point p2 = polygon[i % numVertices];

        if (y > min(p1.y, p2.y)) {
            if (y <= max(p1.y, p2.y)) {
                if (x <= max(p1.x, p2.x)) {
                    double dy = (double)(p2.y - p1.y);
                    if (dy != 0.0) {
                        double xIntersection =
                            ((y - (double)p1.y) * (double)(p2.x - p1.x)) / dy + (double)p1.x;
                        if (p1.x == p2.x || x <= xIntersection) {
                            inside = !inside;
                        }
                    }
                }
            }
        }
        p1 = p2;
    }

    return inside;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // ---- Read input from inputs/inputs09.txt ----
    ifstream infile("inputs/inputs09.txt");
    if (!infile) {
        cerr << "Failed to open inputs/inputs09.txt\n";
        return 1;
    }

    vector<Point> points;
    string line;
    while (getline(infile, line)) {
        if (line.empty()) continue;
        size_t comma = line.find(',');
        if (comma == string::npos) {
            cerr << "Invalid input line: " << line << "\n";
            continue;
        }
        ll x = stoll(line.substr(0, comma));
        ll y = stoll(line.substr(comma + 1));
        points.push_back(Point{x, y});
    }

    if (points.empty()) {
        cout << "Part 1: 0\nPart 2: 0\n";
        return 0;
    }

    int n = (int)points.size();

    // ---- Build boundary: all tiles along the path between consecutive points ----
    boundary.clear();
    for (int i = 0; i < n; ++i) {
        Point p1 = points[i];
        Point p2 = points[(i + 1) % n];

        int dx = (p2.x > p1.x) ? 1 : (p2.x < p1.x ? -1 : 0);
        int dy = (p2.y > p1.y) ? 1 : (p2.y < p1.y ? -1 : 0);

        Point cur = p1;
        boundary.insert(cur);

        while (!(cur.x == p2.x && cur.y == p2.y)) {
            cur.x += dx;
            cur.y += dy;
            boundary.insert(cur);
        }
    }

    // ---- Part 1: max rectangle area with any two red tiles as opposite corners ----
    long long largest = 0;
    for (int i = 0; i < n; ++i) {
        const Point& p1 = points[i];
        for (int j = i + 1; j < n; ++j) {
            const Point& p2 = points[j];

            long long area =
                (ll)(llabs(p2.x - p1.x) + 1LL) * (ll)(llabs(p2.y - p1.y) + 1LL);
            if (area > largest) largest = area;
        }
    }

    // ---- Part 2: only red/green tiles (inside or on loop) ----
    long long largest2 = 0;

    for (int i = 0; i < n; ++i) {
        const Point& p1 = points[i];

        for (int j = i + 1; j < n; ++j) {
            const Point& p2 = points[j];

            long long area =
                (ll)(llabs(p2.x - p1.x) + 1LL) * (ll)(llabs(p2.y - p1.y) + 1LL);

            // quick skip if area can't improve result
            if (area <= largest2) continue;

            // Other two corners
            Point p3{p1.x, p2.y};
            Point p4{p2.x, p1.y};

            // Check both are inside/on the polygon
            if (!pointOnOrInPolygon(p3, points) || !pointOnOrInPolygon(p4, points)) {
                continue;
            }

            // Build rectangle in min/max form
            ll x_min = min(p1.x, p2.x);
            ll x_max = max(p1.x, p2.x);
            ll y_min = min(p1.y, p2.y);
            ll y_max = max(p1.y, p2.y);

            Point rect[4] = {
                {x_min, y_min}, {x_max, y_min},
                {x_max, y_max}, {x_min, y_max}
            };

            // Ensure no rectangle edge properly intersects any polygon edge
            bool ok = true;
            for (int p = 0; p < n && ok; ++p) {
                Point A = points[p];
                Point B = points[(p + 1) % n];

                for (int q = 0; q < 4 && ok; ++q) {
                    Point C = rect[q];
                    Point D = rect[(q + 1) % 4];

                    if (doSegmentsIntersect(A, B, C, D)) {
                        ok = false;
                    }
                }
            }

            if (!ok) continue;

            if (area > largest2) {
                largest2 = area;
            }
        }
    }

    cout << "Part 1: " << largest << "\n";
    cout << "Part 2: " << largest2 << "\n";

    return 0;
}
