#include<iostream>
#include<fstream>
#include<string>
#include<vector>

using namespace std;

unordered_map<int,unordered_map<int,long long>> memo;

long long solve(vector<string> &grid,int activeBeamPosition = 0,int index = 0){
    if(index == grid.size()){
        return 1;
    }

    if (activeBeamPosition < 0 || activeBeamPosition >= (int)grid[index].size()) {
        return 0;
    }

    if(memo.find(index) != memo.end() && memo[index].find(activeBeamPosition) != memo[index].end()){
        return memo[index][activeBeamPosition];
    }
    
    // We process string at current index.
    long long left,right;
    left = right = 0; 
    
    for(int i=0;i<grid[index].size();i++){
        // We encounter a splitter (^)
        // We check whether position of this splitter aligns with activeBeamPosition.
        // If it aligns, we create two separate timelines
        // One in which the beam goes left, and in the other the beam will go right.
        // Because only 1 position per row will have an active beam position, we can break out of the loop.
        // Each split represents a timeline, hence we will add 1 to both the left path and the right path to represent each timeline.
        if(grid[index][i] == '^' && i == activeBeamPosition){
            left = solve(grid,activeBeamPosition-1,index+1);
            right = solve(grid,activeBeamPosition+1,index+1);
        }
    }

    if(left == right && left == 0){
        return memo[index][activeBeamPosition] = solve(grid,activeBeamPosition,index+1);
    }

    return memo[index][activeBeamPosition] = left+right;
}

int main(){

    string originBeam = "......................................................................S......................................................................";
    fstream infile("inputs/inputs07.txt");
    string line;
    vector<string> grid;

    while(getline(infile,line)){
        grid.push_back(line);
    }

    cout<<solve(grid,originBeam.find('S'))<<endl;

    return 0;
}