#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

// stringstream is used to treat a string as an input stream like cin (reading input from keyboard) or ifstream (reading input from file).
// Its uses involve easily parsing and breaking strings basis a condition (by default by ' ') 
// as is the behaviour with cin (cin won't read further input from keyboard if it encounters a space character.)

// Part 1 : Break a string into words separated by spaces.
vector<string> separateBySpace(string &s){
    vector<string> res;
    stringstream ss(s);
    string word;

    while(ss >> word) {
        res.push_back(word);
    }

    return res;
}

// Part 2 : Split a string using a custom delimiter.

vector<string> separateUsingCustomDelimiter(string& s,char delimiter){
    vector<string> res;
    stringstream ss(s);
    string token;

    while(getline(ss,token,delimiter)){
        res.push_back(token);
    }

    return res;
}

// Part 3 : Attempt https://leetcode.com/problems/reverse-words-in-a-string/ using stack and string stream.
// Add-on - attempt its inplace counterpart. Don't use extra space to store string's state. Its a two pointer type problem.

// Part 4 : Breaking based on a condition - Write a function that takes a string as an input and outputs the contents of a string broken by spaces until an integer appears.
void printTillNum(string& s){
    stringstream ss(s);
    string token;

    while(ss >> token){
        if(token[0] >= '0' && token[0] <= '9') break;
        cout<<token<<endl;
    }
}

// Part 5 : Mixed extraction
struct Person {
    public:
    string name;
    int age;
    float weight;
};

Person* extractPerson(string& s){
    stringstream ss(s);
    Person *p = new Person();

    ss>>p->name>>p->age>>p->weight; // this also supports implict type casting.

    return p;
}

// Part 6 : Handling malformed inputs
void handleInput(string& s){
    stringstream ss(s);
    int x;

    while(ss >> x){
        cout<<x<<endl; // will stop at parsing failure "abc"
    }
}

// Combining getline and stringstream.
void getlineStringstream(string& s){
    stringstream ss(s);
    string id,name,age,city;

    getline(ss,id,',');
    getline(ss,name,',');
    getline(ss,age,',');
    getline(ss,city,',');

    cout<<id<<":"<<name<<":"<<age<<":"<<city<<endl;
}

// How to clear a stringstream ?
// Use ss.str(""); ss.clear();

int main() {

    string s = "hello world from cplusplus"; // Input for part 1.
    string s1 = "apple,banana,grapes,orange"; // Input for part 2.
    string s2 = "apple banana 123"; // Input for part 3.
    string s3 = "John 25 72.5";
    string s4 = "10 30 abc 40";
    string s5 = "101,John,25,Texas";

    vector<string> v = separateBySpace(s);
    vector<string> v1 = separateUsingCustomDelimiter(s1,',');
    // printTillNum(s2);
    // Person *p = extractPerson(s3);
    // cout<<p->name<<" "<<p->age<<" "<<p->weight<<endl;
    // delete p;
    // handleInput(s4);
    getlineStringstream(s5);

    // for(int i=0;i<20;i++) cout<<"_";
    // cout<<endl;
    // cout<<"Output for first part"<<endl;
    // for(int i=0;i<20;i++) cout<<"_";
    // cout<<endl<<endl;


    // for(auto &i : v){
    //     cout<<i<<endl;
    // }

    
    // for(int i=0;i<20;i++) cout<<"_";
    // cout<<endl;
    // cout<<"Output for second part"<<endl;
    // for(int i=0;i<20;i++) cout<<"_";
    // cout<<endl<<endl;

    // for(auto &i : v1){
    //     cout<<i<<endl;
    // }

    return 0;
}
