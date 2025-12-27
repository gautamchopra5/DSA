#ifndef SORTING_TECHNIQUES_H
#define SORTING_TECHNIQUES_H
#include <vector>

class SortingTechniques {
public:
    SortingTechniques();
    ~SortingTechniques();
    void bubbleSort(std::vector<int>& arr);
    void insertionSort(std::vector<int>& arr);
    void selectionSort(std::vector<int>& arr);
    void merge(std::vector<int>& arr, int left, int right);
    void quickSort(std::vector<int>& arr,int left, int right);
    private:
    int Partition(std::vector<int>& arr, int left, int right);
    void mergeSort(std::vector<int>& arr,int left,int mid,int right);
};

#endif
