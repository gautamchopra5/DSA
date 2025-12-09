#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<algorithm>

#define ll long long

using namespace std;

int main() {
    fstream infile("inputs/inputs09.txt");
    string line;
    vector<pair<ll,ll>> coordinates;

    while(getline(infile,line)){
        ll x = stoll(line.substr(0,line.find(",")));
        ll y = stoll(line.substr(line.find(",")+1,line.size()));

        coordinates.push_back({x,y});
    }

    long long maxArea = 0;

    for(int i=0;i<coordinates.size();i++){
        for(int j=i+1;j<coordinates.size();j++){
            maxArea = max(maxArea,((abs(coordinates[i].first-coordinates[j].first))+1)*(abs(coordinates[i].second-coordinates[j].second)+1));
        }
    }

    cout<<maxArea<<endl;

    return 0;
}