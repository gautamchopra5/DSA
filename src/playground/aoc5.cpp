#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>

using namespace std;

bool inRange(pair<long long,long long> &p1,pair<long long,long long> &p2){

    if(p1.second >= p2.first){
        return true;
    }

    return false;
}

bool isFreshIngredient(vector<pair<long long,long long>> &ranges,long long ingredient){
    
    for(auto &i : ranges){
        if(ingredient >= i.first && ingredient <= i.second) return true;
    }

    return false;
}

vector<pair<long long,long long>> mergeIntervals(vector<pair<long long,long long>> &ranges){

    vector<pair<long long,long long>> res;

    int i = 0,j = 1;

    while(j < ranges.size() && i < ranges.size()){
        pair<long long,long long> temp = ranges[i];
        int j = i+1;

        while(j < ranges.size() && inRange(temp,ranges[j])){
            temp.second = max(temp.second,ranges[j].second);
            j++;
        }

        res.push_back(temp);
        i = j;
    }

    return res;
}

int main(){
    fstream infile("inputs/inputs05-ranges.txt");
    string line;
    vector<pair<long long,long long>> ranges;

    auto lambda = [&](pair<long long,long long> &p1,pair<long long,long long> &p2){
        return p1.first<p2.first;
    };

    while(getline(infile, line)){
        int pos = line.find('-');

        long long r1 = stoll(line.substr(0,pos));
        long long r2 = stoll(line.substr(pos+1,line.length()));

        ranges.push_back({r1,r2});
    }

    std::sort(ranges.begin(),ranges.end(),lambda);

    vector<pair<long long,long long>> processedIntervals = mergeIntervals(ranges);

    fstream infile2("inputs/inputs05-ingredient-checklist.txt");
    int res = 0;

    while(getline(infile2, line)){
        long long num = stoll(line);

        if(isFreshIngredient(processedIntervals,num)) {
            res++;
        }
    }

    cout<<res<<endl;

    return 0;
}