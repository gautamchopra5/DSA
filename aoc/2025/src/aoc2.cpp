#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

bool isInvalid(long long x) {
    string s = to_string(x);
    int n = s.size();

    for (int len = 1; len <= n / 2; ++len) {
        if (n % len != 0) continue;

        string block = s.substr(0, len);
        bool ok = true;

        for (int i = len; i < n; i += len) {
            if (s.compare(i, len, block) != 0) {
                ok = false;
                break;
            }
        }

        if (ok) return true;
    }

    return false;
}

int main() {
    ifstream infile("inputs/inputs02.txt");
    string line;

    getline(infile, line);

    stringstream ss(line);
    string token;

    vector<long long> invalidIds;

    while (getline(ss, token, ',')) {
        size_t dashPos = token.find('-');
        string r1 = token.substr(0, dashPos);
        string r2 = token.substr(dashPos + 1);

        long long num1 = stoll(r1);
        long long num2 = stoll(r2);

        for (long long i = num1; i <= num2; ++i) {
            if (isInvalid(i)) {
                invalidIds.push_back(i);
            }
        }
    }

    unsigned long long res = 0;
    for (long long id : invalidIds) {
        res += id;
    }

    cout << res << endl;
    return 0;
}
