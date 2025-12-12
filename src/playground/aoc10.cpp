#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <cctype>

using namespace std;

int solveMachine(const string &pattern, const vector<vector<int>> &buttons) {
    int N = pattern.size();
    if (N == 0) return 0;

    // Build target mask (bit i corresponds to light i)
    int target = 0;
    for (int i = 0; i < N; ++i) {
        if (pattern[i] == '#') {
            target |= (1 << i);
        }
    }

    if (target == 0) return 0; // already correct

    // Build button masks
    vector<int> buttonMasks;
    for (const auto &btn : buttons) {
        int mask = 0;
        for (int idx : btn) {
            if (idx >= 0 && idx < N) {
                mask |= (1 << idx);
            }
        }
        if (mask != 0) {
            buttonMasks.push_back(mask);
        }
    }

    if (buttonMasks.empty()) {
        return (target == 0) ? 0 : -1; // impossible
    }

    // BFS over all states 0..(1<<N)-1
    int maxStates = 1 << N;
    vector<int> dist(maxStates, -1);
    queue<int> q;

    dist[0] = 0;
    q.push(0);

    while (!q.empty()) {
        int s = q.front();
        q.pop();
        int d = dist[s];

        for (int mask : buttonMasks) {
            int ns = s ^ mask;
            if (dist[ns] == -1) {
                dist[ns] = d + 1;
                if (ns == target) {
                    return dist[ns]; // minimum presses for this machine
                }
                q.push(ns);
            }
        }
    }

    // Unreachable
    return -1;
}

int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ifstream infile("inputs/inputs10.txt");
    
    if (!infile) {
        cerr << "Error: could not open file " << argv[1] << "\n";
        return 1;
    }

    string line;
    long long totalPresses = 0;

    while (getline(infile, line)) {
        if (line.empty()) continue;

        // Find pattern inside [ ... ]
        size_t lb = line.find('[');
        size_t rb = line.find(']');
        if (lb == string::npos || rb == string::npos || rb <= lb + 1) {
            continue; // skip malformed lines
        }

        string pattern = line.substr(lb + 1, rb - lb - 1);

        // Parse buttons ( ... ) after the pattern, stopping at '{'
        vector<vector<int>> buttons;
        size_t pos = rb + 1;

        while (pos < line.size()) {
            if (line[pos] == '{') {
                // Joltage requirements start here; ignore the rest of the line
                break;
            }

            if (line[pos] == '(') {
                size_t close = line.find(')', pos);
                if (close == string::npos) {
                    break; // malformed, stop parsing this line
                }

                string inside = line.substr(pos + 1, close - pos - 1);
                vector<int> btn;
                string num;

                for (char ch : inside) {
                    if (isdigit(ch) || ch == '-') {
                        num.push_back(ch);
                    } else if (ch == ',' || isspace(ch)) {
                        if (!num.empty()) {
                            btn.push_back(stoi(num));
                            num.clear();
                        }
                    }
                }
                if (!num.empty()) {
                    btn.push_back(stoi(num));
                    num.clear();
                }

                if (!btn.empty()) {
                    buttons.push_back(btn);
                }

                pos = close + 1;
            } else {
                ++pos;
            }
        }

        int presses = solveMachine(pattern, buttons);
        if (presses < 0) {
            cerr << "Warning: no solution for machine line:\n" << line << "\n";
            // Depending on puzzle rules, you might want to treat this as error.
            continue;
        }

        totalPresses += presses;
    }

    cout << totalPresses << "\n";
    return 0;
}
