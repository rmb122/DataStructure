#include <vector>
#include <iostream>

using std::vector;

void insertSort(vector<int> &arr) {
    for(int i = 1; i < arr.size(); i++) {
        int temp = arr[i];
        int j = i - 1;
        for(; j >= 0 && arr[j] > temp; j--) {
            arr[j + 1] = arr[j];
        }
        arr[j + 1] = temp;
    }
}

void shellSort(vector<int> &arr) {
    int step = 1;
    while(step < arr.size() / 2) {
        step *= 2;
    }
    while(step >= 1) {
        for(int i = step; i < arr.size(); i++) {
            int temp = arr[i];
            int j = i - step;
            for(; j >= 0 && arr[j] > temp; j -= step) {
                arr[j + step] = arr[j];
            }
            arr[j + step] = temp;
        }
        step /= 2;
    }
}