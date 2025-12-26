#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>

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

int main(){
    // Parse numbers file.
    fstream infile("inputs/inputs06.txt");
    string line;

    vector<vector<long long>> grid;

    while(getline(infile,line)){
        trim(line);

        int i = 0, j = 0;

        vector<long long> temp;

        while(j < line.length()){
            if(line[i] != ' '){
                if(line[j] != ' ') j++;
                else {
                    temp.push_back(stoll(line.substr(i,j-i)));
                    i = j;
                }
            } else {
                if(line[j] == ' ') j++;
                else {
                    i = j;
                }
            }      
        }

        if(j == line.length() && i < line.length()){
            if(line[i] != ' '){
                temp.push_back(stoll(line.substr(i,j-i)));
            }
        }

        grid.push_back(temp);
    }

    fstream inFileOperations("inputs/inputs06-operations.txt");
    vector<string> operations;
    
    while(getline(inFileOperations,line)){
        trim(line);

        int i = 0, j = 0;

        while(j < line.length()){
            if(line[i] != ' '){
                if(line[j] != ' ') j++;
                else {
                    operations.push_back(line.substr(i,j-i));
                    i = j;
                }
            } else {
                if(line[j] == ' ') j++;
                else {
                    i = j;
                }
            }      
        }

        if(j == line.length() && i < line.length()){
            if(line[i] != ' '){
                operations.push_back(line.substr(i,j-i));
            }
        }
    }

    long long finRes = 0;

    for(int i=0;i<operations.size();i++){
        long long temp;

        if(operations[i] == "+") temp = 0;
        else temp = 1;

        for(int j=0;j<grid.size();j++){
            if(operations[i] == "+"){
                temp += grid[j][i];
            } else {
                temp *= grid[j][i];
            }
        }

        finRes += temp;
    }

    cout<<finRes<<endl;


    return 0;
}