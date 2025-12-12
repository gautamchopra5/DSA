#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<queue>
#include<cctype>
#include<climits>
#include<numeric>

using namespace std;

// Helper: parse a comma-separated list of ints from a substring
vector<int> parseIntList(const string &s) {
    vector<int> res;
    string num;
    for (char ch : s) {
        if (isdigit(ch) || ch == '-') {
            num.push_back(ch);
        } else if (ch == ',' || isspace(ch)) {
            if (!num.empty()) {
                res.push_back(stoi(num));
                num.clear();
            }
        }
    }
    if (!num.empty()) {
        res.push_back(stoi(num));
        num.clear();
    }
    return res;
}

// Solver for one machine's joltage system using DFS + branch-and-bound
struct JoltageSolver {
    int m; // number of counters
    int k; // number of buttons

    vector<int> target;                // required jolts per counter
    vector<vector<int>> buttons;       // filtered button indices (0..m-1)
    vector<int> lastIdx;               // last button index that touches each counter
    int maxGain;                       // max number of counters affected by any button
    long long best;                    // best (minimum) total presses found

    JoltageSolver(const vector<int> &t, vector<vector<int>> b)
        : target(t), buttons(move(b)) {

        m = (int)target.size();
        k = (int)buttons.size();
        best = LLONG_MAX;

        if (m == 0) {
            maxGain = 1;
            lastIdx.assign(0, -1);
            return;
        }

        // Filter out indices >= m from buttons
        for (auto &btn : buttons) {
            vector<int> filtered;
            filtered.reserve(btn.size());
            for (int idx : btn) {
                if (0 <= idx && idx < m) filtered.push_back(idx);
            }
            sort(filtered.begin(), filtered.end());
            filtered.erase(unique(filtered.begin(), filtered.end()), filtered.end());
            btn.swap(filtered);
        }

        // Optional: sort buttons by decreasing size to get better pruning
        vector<int> order(k);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int a, int b) {
            return buttons[a].size() > buttons[b].size();
        });

        vector<vector<int>> newButtons;
        newButtons.reserve(k);
        for (int idx : order) newButtons.push_back(buttons[idx]);
        buttons.swap(newButtons);

        // Compute lastIdx and maxGain after reordering
        lastIdx.assign(m, -1);
        maxGain = 1;
        for (int j = 0; j < k; ++j) {
            const auto &btn = buttons[j];
            if (!btn.empty())
                maxGain = max<int>(maxGain, (int)btn.size());
            for (int c : btn) {
                if (0 <= c && c < m) {
                    lastIdx[c] = max(lastIdx[c], j);
                }
            }
        }
    }

    // depth-first search with pruning
    void dfs(int idx, long long curPresses, vector<int> &remaining) {
        if (curPresses >= best) return;

        long long totalRem = 0;
        for (int x : remaining) {
            if (x < 0) return;          // overshoot
            totalRem += x;
        }
        if (totalRem == 0) {
            best = min(best, curPresses);
            return;
        }
        if (idx == k) return;          // no more buttons

        // If any remaining counter can't be affected by any future button, prune
        for (int i = 0; i < m; ++i) {
            if (remaining[i] > 0 && lastIdx[i] < idx) {
                return;
            }
        }

        // Lower bound on additional presses assuming each press affects maxGain counters
        long long lbExtra = (totalRem + maxGain - 1) / maxGain;
        if (curPresses + lbExtra >= best) return;

        const auto &btn = buttons[idx];
        if (btn.empty()) {
            // Button doesn't change any counter; skip it
            dfs(idx + 1, curPresses, remaining);
            return;
        }

        // Maximum times we can press this button without overshooting any touched counter
        int maxPress = INT_MAX;
        for (int c : btn) {
            maxPress = min(maxPress, remaining[c]);
        }
        if (maxPress < 0) return;
        if (maxPress == INT_MAX) {
            // Should not really happen, but just in case
            dfs(idx + 1, curPresses, remaining);
            return;
        }

        // Try pressing this button p times (0..maxPress)
        // Explore small p first to hopefully find a good solution early.
        for (int p = 0; p <= maxPress; ++p) {
            vector<int> nextRem = remaining;
            if (p > 0) {
                for (int c : btn) {
                    nextRem[c] -= p;
                }
            }
            dfs(idx + 1, curPresses + p, nextRem);
        }
    }

    long long solve() {
        if (m == 0) return 0;

        // Quick impossibility check: a counter with positive target but no button ever touches it
        for (int i = 0; i < m; ++i) {
            if (target[i] > 0 && lastIdx[i] == -1) {
                return -1;
            }
        }

        vector<int> remaining = target;
        dfs(0, 0, remaining);
        return (best == LLONG_MAX ? -1 : best);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ifstream infile("inputs/inputs10.txt");
    if (!infile) {
        cerr << "Error: could not open inputs/input10.txt\n";
        return 1;
    }

    string line;
    long long totalPresses = 0;

    while (getline(infile, line)) {
        if (line.empty()) continue;

        // Find joltage block { ... }
        size_t lcur = line.find('{');
        size_t rcur = line.find('}');
        if (lcur == string::npos || rcur == string::npos || rcur <= lcur + 1) {
            continue; // skip malformed lines
        }
        string joltsStr = line.substr(lcur + 1, rcur - lcur - 1);
        vector<int> jolts = parseIntList(joltsStr);
        if (jolts.empty()) continue;

        // Between ']' and '{' we have the buttons
        size_t rbr = line.find(']');
        if (rbr == string::npos) continue;

        string mid = line.substr(rbr + 1, lcur - (rbr + 1));
        vector<vector<int>> buttons;

        // Extract all "( ... )" blocks
        size_t pos = 0;
        while (pos < mid.size()) {
            if (mid[pos] == '(') {
                size_t close = mid.find(')', pos);
                if (close == string::npos) break;
                string inside = mid.substr(pos + 1, close - pos - 1);
                buttons.push_back(parseIntList(inside));
                pos = close + 1;
            } else {
                ++pos;
            }
        }

        JoltageSolver solver(jolts, buttons);
        long long presses = solver.solve();
        if (presses < 0) {
            cerr << "Warning: no solution for line (joltage):\n" << line << "\n";
        } else {
            totalPresses += presses;
        }
    }

    cout << totalPresses << "\n";
    return 0;
}
