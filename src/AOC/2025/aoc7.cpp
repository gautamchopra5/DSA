#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<unordered_set>

using namespace std;

int main(){
    string originBeam = "......................................................................S......................................................................";
    fstream infile("inputs/inputs07.txt");
    string line;
    unordered_set<int> activeBeamPositions;
    int splitCount = 0;

    activeBeamPositions.insert(originBeam.find('S'));

    while(getline(infile,line)){
        for(int i=0;i<line.length();i++){
            if(line[i] == '^'){
                // We encounter a splitter
                // We check if the current position is an active beam position
                // If it is active, we split the beam and increment count of splits by 1.
                // We erase current position from activeBeamPositions and add positions i-1 and i+1 to activeBeamPositions.

                if(activeBeamPositions.find(i) != activeBeamPositions.end()){
                    splitCount++;
                    activeBeamPositions.erase(i);
                    activeBeamPositions.insert(i+1);
                    activeBeamPositions.insert(i-1);
                }
            }
        }
    }

    cout<<splitCount<<endl;

    return 0;
}