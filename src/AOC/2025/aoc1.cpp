#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <fstream>

using namespace std;

int main() {
    ifstream infile("inputs/inputs.txt");
    vector<string> lines;
    string line;

    while(getline(infile, line)) {
        lines.push_back(line);
    }

    int currPos = 50, res = 0;

    for(string s : lines) {
        char dir = s[0];
        int val = stoi(s.substr(1, s.length() - 1));
        int nZero = 0;

        if(dir == 'L') {
            nZero = (val/100);

            int temp = val%100;

            if(currPos != 0 && currPos - temp <= 0) {
                nZero += 1;
            }

            currPos = (currPos - val) % 100;

            if(currPos < 0){
                currPos += 100;
            }
        } else {
            nZero = (currPos + val) / 100;
            currPos = (currPos + val) % 100;
        }

        res += nZero;
    }

    cout << res << endl;
    return 0;
}
