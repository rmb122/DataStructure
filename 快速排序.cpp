#include <vector>
#include <iostream>

using std::vector;

void quickSort(vector<int> &arr, unsigned int startPos, unsigned int endPos) {
    if(startPos >= endPos || endPos == UINT32_MAX) {
        return;
    }

    int mid = arr[startPos];
    unsigned int front = startPos;
    unsigned int behind = endPos;

    while(front < behind) {
        while(front < behind && arr[behind] > mid) { //寻找一个小数
            behind--;
        }
        if(front < behind) {
            arr[front] = arr[behind];
            front++;
        }
        while(front < behind && arr[front] < mid) { //寻找一个大数
            front++;
        }
        if(front < behind) {
            arr[behind] = arr[front];
            behind--;
        }
    }
    arr[front] = mid;

    quickSort(arr, startPos, front - 1);
    quickSort(arr, front + 1, endPos);
}