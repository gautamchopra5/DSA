#include "SortingTechniques.h"
#include<vector>
using namespace std;

SortingTechniques::SortingTechniques() = default;

void SortingTechniques::bubbleSort(std::vector<int>& arr) {

    for(int i=0;i<arr.size();i++){
        bool swapped = 0;

        for(int j=0;j<arr.size()-i-1;j++){
            if(arr[j] > arr[j+1]){
                swapped = 1;
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }

        if(!swapped) break;
    }

}

void SortingTechniques::selectionSort(std::vector<int>& arr) {

    for(int i=0;i<arr.size();i++){
        for(int j=i+1;j<arr.size();j++){
            if(arr[i] > arr[j]){
                std::swap(arr[i],arr[j]);
            }
        }
    }

}

void SortingTechniques::insertionSort(std::vector<int> &arr) {

    for(int i=0;i<arr.size();i++){
        
        int j = i;

        while(j > 0 && arr[j] < arr[j-1]){
            std::swap(arr[j],arr[j-1]);
            j--;
        }
    }
}

void SortingTechniques::mergeSort(std::vector<int>& arr,int left,int mid,int right) {
    if(left >= right) return;

    int i,j;
    i = left,j = mid+1;
    vector<int> buffer;

    while(i <= mid || j <= right){
        if(i == mid+1){
            buffer.push_back(arr[j]);
            j++;
        } else if(j == right+1){
            buffer.push_back(arr[i]);
            i++;
        } else {
            if(arr[i] < arr[j]){
                buffer.push_back(arr[i]);
                i++;
            } else if(arr[j] < arr[i]){
                buffer.push_back(arr[j]);
                j++;
            } else {
                buffer.push_back(arr[i]);
                buffer.push_back(arr[j]);
                i++;
                j++;
            }
        }
    }

    int index = 0;

    for(int i=left;i<=right;i++){
        arr[i] = buffer[index];
        index++;
    }

}

void SortingTechniques::merge(std::vector<int>& arr, int left, int right) {
    if(left >= right) return;

    int mid = ((right-left)/2)+left;

    merge(arr,left,mid);
    merge(arr,mid+1,right);
    mergeSort(arr,left,mid,right);
}

void SortingTechniques::quickSort(vector<int>& arr,int left,int right){
    if(left >= right) return;

    int partitionIndex = Partition(arr,left,right);
    quickSort(arr,left,partitionIndex-1);
    quickSort(arr,partitionIndex+1,right);
}

int SortingTechniques::Partition(vector<int>& arr,int left,int right){
    int pivotIndex = left;
    left = left+1;

    while(left <= right){
        if(arr[left] <= arr[pivotIndex] && arr[right] >= arr[pivotIndex]){
            left++;
            right--;
        } else if(arr[left] > arr[pivotIndex] && arr[right] >= arr[pivotIndex]){
            right--;
        } else if(arr[left] <= arr[pivotIndex] && arr[right] < arr[pivotIndex]){
            left++;
        } else {
            std::swap(arr[left],arr[right]);
            left++;
            right--;
        }
    }

    std::swap(arr[right],arr[pivotIndex]);
    return right;
}

SortingTechniques::~SortingTechniques() = default;
