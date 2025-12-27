#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    ifstream infile("inputs/inputs03.txt");
    string line;
    long long res = 0;   // use long long to avoid overflow

    while (getline(infile, line)) {
        if (line.empty()) continue;

        int n = (int)line.size();
        int need = 12;          // we must pick 12 digits
        int start = 0;          // first index we can pick from
        string chosen;

        while (need > 0) {
            int end = n - need; // last index we can start from for this pick

            char bestDigit = '0' - 1;  // something smaller than '0'
            int bestIdx = -1;

            // find max digit in [start, end]
            for (int i = start; i <= end; ++i) {
                if (line[i] > bestDigit) {
                    bestDigit = line[i];
                    bestIdx = i;
                }
            }

            chosen.push_back(bestDigit);
            start = bestIdx + 1; // next digit must be after this index
            --need;
        }

        res += stoll(chosen);
    }

    cout << res << endl;
    return 0;
}
