#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <cstdint>
#include <sstream>
#include <climits>

using namespace std;

struct Shape { vector<string> g; };

static string rstrip(string s) {
    while (!s.empty() && (s.back() == '\r' || s.back() == ' ' || s.back() == '\t'))
        s.pop_back();
    return s;
}

static bool isGridLine(const string& s) {
    if (s.empty()) return false;
    for (char ch : s) if (ch != '.' && ch != '#') return false;
    return true;
}

static vector<string> trim_empty_rows_cols(const vector<string>& in) {
    int h = (int)in.size(), w = (int)in[0].size();
    int r0 = h, r1 = -1, c0 = w, c1 = -1;
    for (int r = 0; r < h; r++) for (int c = 0; c < w; c++) {
        if (in[r][c] == '#') {
            r0 = min(r0, r); r1 = max(r1, r);
            c0 = min(c0, c); c1 = max(c1, c);
        }
    }
    if (r1 == -1) return vector<string>(1, "");
    vector<string> out;
    for (int r = r0; r <= r1; r++) out.push_back(in[r].substr(c0, c1 - c0 + 1));
    return out;
}

static vector<string> rotate90(const vector<string>& a) {
    int h = (int)a.size(), w = (int)a[0].size();
    vector<string> b(w, string(h, '.'));
    for (int r = 0; r < h; r++)
        for (int c = 0; c < w; c++)
            b[c][h - 1 - r] = a[r][c];
    return b;
}

static vector<string> flipH(const vector<string>& a) {
    vector<string> b = a;
    for (size_t i = 0; i < b.size(); i++) reverse(b[i].begin(), b[i].end());
    return b;
}

static string canon_key(const vector<string>& a) {
    string s;
    for (size_t i = 0; i < a.size(); i++) { s += a[i]; s += '\n'; }
    return s;
}

static vector<vector<string>> all_unique_orientations(const vector<string>& base) {
    unordered_set<string> seen;
    vector<vector<string>> res;
    vector<string> cur = trim_empty_rows_cols(base);

    for (int f = 0; f < 2; f++) {
        vector<string> x = (f ? flipH(cur) : cur);
        for (int rot = 0; rot < 4; rot++) {
            if (rot) x = rotate90(x);
            vector<string> t = trim_empty_rows_cols(x);
            string k = canon_key(t);
            if (!seen.count(k)) { seen.insert(k); res.push_back(t); }
        }
    }
    return res;
}

struct Placement {
    int type;
    vector<uint64_t> mask; // occupied cells
};

struct RegionSolver {
    int W, H, N, chunks;
    vector<vector<Placement>> placementsByType;
    vector<int> remain;
    vector<uint64_t> filled;

    bool canPlace(const vector<uint64_t>& m) const {
        for (int i = 0; i < chunks; i++) if (filled[i] & m[i]) return false;
        return true;
    }
    void doPlace(const vector<uint64_t>& m) {
        for (int i = 0; i < chunks; i++) filled[i] |= m[i];
    }
    void undoPlace(const vector<uint64_t>& m) {
        for (int i = 0; i < chunks; i++) filled[i] ^= m[i]; // safe (no overlap)
    }

    int chooseTypeMRV() const {
        int bestType = -1;
        int bestOptions = INT_MAX;

        for (int t = 0; t < (int)remain.size(); t++) {
            if (remain[t] == 0) continue;

            int options = 0;
            for (size_t k = 0; k < placementsByType[t].size(); k++) {
                if (canPlace(placementsByType[t][k].mask)) {
                    options++;
                    if (options >= bestOptions) break;
                }
            }

            if (options < bestOptions) {
                bestOptions = options;
                bestType = t;
                if (bestOptions == 0) return bestType;
            }
        }
        return bestType;
    }

    bool dfs() {
        // success: all pieces placed
        for (int t = 0; t < (int)remain.size(); t++)
            if (remain[t] > 0) goto not_done;
        return true;
    not_done:

        int t = chooseTypeMRV();
        if (t == -1) return true; // nothing left
        // if MRV type has zero options => fail
        bool any = false;
        for (size_t k = 0; k < placementsByType[t].size(); k++) {
            if (canPlace(placementsByType[t][k].mask)) { any = true; break; }
        }
        if (!any) return false;

        for (size_t k = 0; k < placementsByType[t].size(); k++) {
            const Placement& p = placementsByType[t][k];
            if (!canPlace(p.mask)) continue;

            remain[t]--;
            doPlace(p.mask);

            if (dfs()) return true;

            undoPlace(p.mask);
            remain[t]++;
        }
        return false;
    }

    bool solve(const vector<vector<vector<string>>>& orientationsByType,
               const vector<int>& need) {

        remain = need;
        filled.assign(chunks, 0ULL);
        placementsByType.assign(need.size(), vector<Placement>());

        // area quick check
        long long totalCells = 0;
        for (int t = 0; t < (int)need.size(); t++) {
            int a = 0;
            const vector<string>& o0 = orientationsByType[t][0];
            for (size_t r = 0; r < o0.size(); r++)
                for (size_t c = 0; c < o0[r].size(); c++)
                    if (o0[r][c] == '#') a++;
            totalCells += 1LL * need[t] * a;
        }
        if (totalCells > N) return false;

        auto makeMask = [&](const vector<pair<int,int>>& cells)->vector<uint64_t>{
            vector<uint64_t> m(chunks, 0ULL);
            for (size_t i = 0; i < cells.size(); i++) {
                int r = cells[i].first, c = cells[i].second;
                int idx = r * W + c;
                m[idx >> 6] |= 1ULL << (idx & 63);
            }
            return m;
        };

        // Build placements (only for types needed)
        for (int t = 0; t < (int)need.size(); t++) {
            if (need[t] == 0) continue;

            for (size_t oi = 0; oi < orientationsByType[t].size(); oi++) {
                const vector<string>& ori = orientationsByType[t][oi];
                int sh = (int)ori.size(), sw = (int)ori[0].size();

                vector<pair<int,int>> rel;
                for (int r = 0; r < sh; r++)
                    for (int c = 0; c < sw; c++)
                        if (ori[r][c] == '#') rel.push_back(make_pair(r,c));

                for (int top = 0; top + sh <= H; top++) {
                    for (int left = 0; left + sw <= W; left++) {
                        vector<pair<int,int>> absCells;
                        absCells.reserve(rel.size());
                        for (size_t i = 0; i < rel.size(); i++) {
                            absCells.push_back(make_pair(top + rel[i].first, left + rel[i].second));
                        }
                        Placement p;
                        p.type = t;
                        p.mask = makeMask(absCells);
                        placementsByType[t].push_back(p);
                    }
                }
            }

            if (placementsByType[t].empty()) return false; // needed but impossible
        }

        return dfs();
    }
};

struct Region { int W, H; vector<int> need; };

static bool isShapeHeader(const string& s) {
    if (s.empty()) return false;
    int j = 0;
    while (j < (int)s.size() && isdigit((unsigned char)s[j])) j++;
    return j > 0 && j < (int)s.size() && s[j] == ':';
}

static bool isRegionLine(const string& s) {
    size_t posx = s.find('x');
    size_t posc = s.find(':');
    if (posx == string::npos || posc == string::npos || posx > posc) return false;
    for (size_t k = 0; k < posx; k++) if (!isdigit((unsigned char)s[k])) return false;
    for (size_t k = posx + 1; k < posc; k++) if (!isdigit((unsigned char)s[k])) return false;
    return true;
}

int main() {
    ifstream in("inputs/inputs12.txt");
    if (!in) { cerr << "Failed to open inputs/inputs12.txt\n"; return 1; }

    vector<string> lines;
    string line;
    while (getline(in, line)) lines.push_back(rstrip(line));

    vector<Shape> shapes;
    vector<Region> regions;

    int i = 0;
    // shapes
    while (i < (int)lines.size()) {
        while (i < (int)lines.size() && lines[i].empty()) i++;
        if (i >= (int)lines.size()) break;
        if (isRegionLine(lines[i])) break;

        if (!isShapeHeader(lines[i])) {
            cerr << "Bad shape header: [" << lines[i] << "]\n";
            return 1;
        }
        i++; // skip "k:"

        while (i < (int)lines.size() && lines[i].empty()) i++;

        vector<string> g;
        while (i < (int)lines.size() && isGridLine(lines[i])) {
            g.push_back(lines[i]);
            i++;
        }
        if (g.empty()) { cerr << "Shape grid missing\n"; return 1; }
        shapes.push_back(Shape{g});
    }

    int S = (int)shapes.size();
    if (S == 0) { cerr << "No shapes parsed\n"; return 1; }

    // regions
    while (i < (int)lines.size()) {
        while (i < (int)lines.size() && lines[i].empty()) i++;
        if (i >= (int)lines.size()) break;

        if (!isRegionLine(lines[i])) {
            cerr << "Bad region line: [" << lines[i] << "]\n";
            return 1;
        }

        string s = lines[i];
        int posx = (int)s.find('x');
        int posc = (int)s.find(':');

        int W = stoi(s.substr(0, posx));
        int H = stoi(s.substr(posx + 1, posc - (posx + 1)));

        vector<int> need;
        string rest = s.substr(posc + 1);
        stringstream ss(rest);
        int x;
        while (ss >> x) need.push_back(x);

        if ((int)need.size() != S) {
            cerr << "Region counts=" << need.size() << " but shapes=" << S << "\n";
            return 1;
        }

        regions.push_back(Region{W, H, need});
        i++;
    }

    // orientations
    vector<vector<vector<string>>> orientationsByType(S);
    for (int t = 0; t < S; t++) orientationsByType[t] = all_unique_orientations(shapes[t].g);

    int ok = 0;
    for (size_t ri = 0; ri < regions.size(); ri++) {
        RegionSolver solver;
        solver.W = regions[ri].W;
        solver.H = regions[ri].H;
        solver.N = solver.W * solver.H;
        solver.chunks = (solver.N + 63) / 64;

        if (solver.solve(orientationsByType, regions[ri].need)) ok++;
    }

    cout << ok << "\n";
    return 0;
}
