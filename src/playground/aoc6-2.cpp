#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

int main() {
    // Parse numbers file as raw lines (keep spaces).
    fstream infile("inputs/inputs06.txt");
    string line;

    vector<string> grid;

    while (getline(infile, line)) {
        grid.push_back(line);
    }

    if (grid.empty()) {
        cerr << "Grid is empty\n";
        return 1;
    }

    // Read operations from second file.
    fstream inFileOperations("inputs/inputs06-operations.txt");
    vector<string> operations;
    
    while (getline(inFileOperations, line)) {
        trim(line);

        int i = 0, j = 0;

        while (j < (int)line.length()) {
            if (line[i] != ' ') {
                if (line[j] != ' ')
                    j++;
                else {
                    operations.push_back(line.substr(i, j - i));
                    i = j;
                }
            } else {
                if (line[j] == ' ')
                    j++;
                else {
                    i = j;
                }
            }
        }

        if (j == (int)line.length() && i < (int)line.length()) {
            if (line[i] != ' ') {
                operations.push_back(line.substr(i, j - i));
            }
        }
    }

    // === Part 2: parse the grid column-wise ===

    // 1. Normalize all rows to the same length (pad with spaces)
    size_t numRows = grid.size();
    size_t numCols = 0;
    for (const auto &row : grid) {
        numCols = max(numCols, row.size());
    }
    for (auto &row : grid) {
        if (row.size() < numCols) {
            row.resize(numCols, ' ');
        }
    }

    long long finalResult = 0;
    size_t opIndex = 0;
    vector<long long> currentNumbers;

    // Helper to flush a "group" of column-numbers when we hit a separator column
    auto flushGroup = [&]() {
        if (currentNumbers.empty()) return;
        if (opIndex >= operations.size()) {
            cerr << "Warning: more groups than operations; ignoring extra group.\n";
            return;
        }

        char op = operations[opIndex][0]; // assume "+" or "*"
        long long acc = (op == '+') ? 0LL : 1LL;

        for (long long val : currentNumbers) {
            if (op == '+') acc += val;
            else acc *= val;
        }

        finalResult += acc;
        currentNumbers.clear();
        opIndex++;
    };

    // 2. Process each column
    for (size_t c = 0; c < numCols; ++c) {
        int rows = (int)numRows;

        // Precompute: for each row r, what's the next row >= r with a digit in this column?
        vector<int> nextDigit(rows + 1, -1);
        nextDigit[rows] = -1;

        for (int r = rows - 1; r >= 0; --r) {
            char ch = grid[r][c];
            if (std::isdigit(static_cast<unsigned char>(ch))) {
                nextDigit[r] = r;
            } else {
                nextDigit[r] = nextDigit[r + 1];
            }
        }

        string numStr;
        bool building = false;

        for (int r = 0; r < rows; ++r) {
            char ch = grid[r][c];

            if (!building) {
                // Not yet started building a number in this column
                if (std::isdigit(static_cast<unsigned char>(ch))) {
                    building = true;
                    numStr.push_back(ch);
                }
            } else {
                // Already building a number
                if (std::isdigit(static_cast<unsigned char>(ch))) {
                    numStr.push_back(ch);
                } else {
                    // Space while building: decide if it's a gap (0) or trailing
                    if (nextDigit[r + 1] != -1) {
                        // There is another digit below => gap, append '0'
                        numStr.push_back('0');
                    } else {
                        // No more digits below => trailing, stop building this number
                        break;
                    }
                }
            }
        }

        if (!numStr.empty()) {
            // This column produced a number
            long long val = stoll(numStr);
            currentNumbers.push_back(val);
        } else {
            // This column has NO digits => separator column
            flushGroup();
        }
    }

    // Flush last group if any
    flushGroup();

    cout << finalResult << endl;

    return 0;
}
