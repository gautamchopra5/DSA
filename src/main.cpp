#include<iostream>
#include<SortingTechniques.h>
using namespace std;

int main() {
    vector<int> v = {1,2,2,6,3,5,9,8}; // 3 inversions - (6,3),(6,5),(9,8)
    SortingTechniques *s = new SortingTechniques();
    // s->bubbleSort(v);
    // s->insertionSort(v);
    // s->selectionSort(v);
    // Lets add definitions for merge sort and quick sort.
    // s->merge(v,0,v.size()-1);
    s->quickSort(v,0,v.size()-1);

    for(auto i:v){
        cout<<i<<endl;
    }

    return 0;
}