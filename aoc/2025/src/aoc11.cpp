#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<sstream>
#include<unordered_map>

using namespace std;

// key = node|seenDac|seenFft  (e.g. "ccc|1|0")
long long dfs(const string &curr,
              unordered_map<string, vector<string>> &adjList,
              bool seenDac,
              bool seenFft,
              unordered_map<string, long long> &memo) {

    // If we've reached out, count this path only if it has seen both.
    if (curr == "out") {
        return (seenDac && seenFft) ? 1LL : 0LL;
    }

    // Update flags based on current node
    if (curr == "dac") seenDac = true;
    if (curr == "fft") seenFft = true;

    // Build memo key
    string key = curr + "|" + (seenDac ? "1" : "0") + "|" + (seenFft ? "1" : "0");

    auto it = memo.find(key);
    if (it != memo.end()) {
        return it->second;
    }

    long long res = 0;

    for (const auto &neighbor : adjList[curr]) {
        res += dfs(neighbor, adjList, seenDac, seenFft, memo);
    }

    memo[key] = res;
    return res;
}

int main() {
    fstream infile("inputs/inputs11.txt");
    string line;
    unordered_map<string, vector<string>> adjList;

    while (getline(infile, line)) {
        if (line.empty()) continue;

        stringstream s(line);
        string l;
        string index;

        while (s >> l) {
            if (l.find(":") != string::npos) {
                index = l.substr(0, l.size() - 1); // strip colon
            } else {
                adjList[index].push_back(l);
            }
        }
    }

    unordered_map<string, long long> memo;
    cout << dfs("svr", adjList, false, false, memo) << endl;
    return 0;
}
