#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<queue>

using namespace std;

vector<pair<int,int>> directions = {
    {1,0}, {0,1}, {-1,0}, {0,-1},
    {1,1}, {1,-1}, {-1,1}, {-1,-1}
};

int bfs(vector<string>& grid){
    vector<vector<bool>> visited(grid.size(), vector<bool>(grid[0].size(), false));
    int count = 0;

    queue<pair<int,int>> q;

    q.push({0,0});

    while(!q.empty()){
        pair<int,int> p = q.front();
        q.pop();
        
        int x = p.first;
        int y = p.second;

        visited[x][y] = true;
        // We must check if this is a cell equal to @ or not.
        // If it is equal to @, we check at least 4 neighbors are @.
        // If 4 or more neighbors are @, we cannot process the current cell.
        // If less than 4 neighbors are @, we increment the count.
        if(grid[x][y] == '@'){
            bool allAt = true;
            int atCount = 0;
            
            for(auto& dir: directions){
                int nx = x + dir.first;
                int ny = y + dir.second;
                if(nx >= 0 && nx < grid.size() && ny >= 0 && ny < grid[0].size()){
                    if(grid[nx][ny] == '@'){
                        atCount++;
                    }
                }
            }
            if(atCount < 4){
                count++;
            }
        }

        for(auto& dir: directions){
            int nx = x + dir.first;
            int ny = y + dir.second;
            if(nx >= 0 && nx < grid.size() && ny >= 0 && ny < grid[0].size()){
                if(!visited[nx][ny]){
                    q.push({nx, ny});
                    visited[nx][ny] = true;
                }
            }
        }
    }

    return count;
}

int main() {
    fstream infile("inputs/inputs04.txt");
    string line;
    vector<string> grid;

    while(getline(infile,line)){
        grid.push_back(line);
    }

    int result = bfs(grid);
    // for(auto& row: grid){
    //     cout << row << endl;
    // }

    cout << result << endl;

    return 0;
}