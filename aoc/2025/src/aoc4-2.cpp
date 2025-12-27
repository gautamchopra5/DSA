#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <chrono>

using namespace std;

vector<pair<int,int>> directions = {
    {1,0}, {0,1}, {-1,0}, {0,-1},
    {1,1}, {1,-1}, {-1,1}, {-1,-1}
};

int bfs(vector<string>& grid) {
    int n = grid.size();
    int m = grid[0].size();
    vector<vector<bool>> visited(n, vector<bool>(m, false));
    queue<pair<int,int>> q;

    q.push({0, 0});
    visited[0][0] = true;

    int count = 0;

    while (!q.empty()) {
        pair<int,int> p = q.front();
        int x = p.first;
        int y = p.second;
        q.pop();

        if (grid[x][y] == '@') {
            int atCount = 0;
            for (auto& dir : directions) {
                int nx = x + dir.first;
                int ny = y + dir.second;
                if (nx >= 0 && nx < n && ny >= 0 && ny < m) {
                    if (grid[nx][ny] == '@') { // only '@' if that's your rule
                        atCount++;
                    }
                }
            }
            if (atCount < 4) {
                grid[x][y] = 'x';  // mark processed
                count++;
            }
        }

        for (auto& dir : directions) {
            int nx = x + dir.first;
            int ny = y + dir.second;
            if (nx >= 0 && nx < n && ny >= 0 && ny < m && !visited[nx][ny]) {
                visited[nx][ny] = true;
                q.push({nx, ny});
            }
        }
    }

    return count; // number of cells changed to 'x'
}

int main() {
    auto start = chrono::high_resolution_clock::now();
    
    ifstream infile("inputs/inputs04.txt");
    string line;
    vector<string> grid;

    while (getline(infile, line)) {
        if (!line.empty())
            grid.push_back(line);
    }

    int res = 0;

    while (true) {
        int changed = bfs(grid);
        if (changed == 0) break; // stop when no more changes
        res += changed;
    }

    cout << res << endl;
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "Execution time: " << duration.count() << " microseconds (" 
         << duration.count() / 1000.0 << " ms)" << endl;
    
    return 0;
}
