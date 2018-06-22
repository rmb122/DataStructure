#include <vector>

using namespace std;

void merge(vector<int> &arr, vector<int> &tempArr, unsigned int &start, unsigned int &mid, unsigned int &end) {
    unsigned int currFront = start;
    unsigned int currBehind = mid;
    unsigned int currTemp = start;
    while(currFront < mid && currBehind < end) {
        if(arr[currBehind] < arr[currFront]) {
            tempArr[currTemp++] = arr[currBehind++];
        } else {
            tempArr[currTemp++] = arr[currFront++];
        }
    }
    while(currFront < mid) {
        tempArr[currTemp++] = arr[currFront++];
    }
    while(currBehind < end) {
        tempArr[currTemp++] = arr[currBehind++];
    }
    for(unsigned int i = start; i < end; i++) {
        arr[i] = tempArr[i];
    }
}

void divide(vector<int> &arr, vector<int> &tempArr, unsigned int start, unsigned int end) {
    unsigned int mid = (start + end) / 2;
    if(start < mid) {
        divide(arr, tempArr, start, mid);
        divide(arr, tempArr, mid, end);
        merge(arr, tempArr, start, mid, end);
    }
}

void mergeSort(vector<int> &arr, unsigned int start, unsigned int end) {
    vector<int> tempArr(arr.size(), 0);
    divide(arr, tempArr, start, end);
}