#include <vector>
#include <iostream>

using std::vector;

void quickSort(vector<int> &arr, unsigned int startPos, unsigned int endPos) {
    if(startPos + 1 >= endPos || endPos == std::numeric_limits<unsigned int>::max()) {
        if(startPos + 1 == endPos) {
            if(arr[startPos] > arr[startPos + 1]) {
                arr[startPos] ^= arr[startPos + 1];
                arr[startPos + 1] ^= arr[startPos];
                arr[startPos] ^= arr[startPos + 1];
            }
        }
        return;
    }

    int mid = arr[startPos];
    unsigned int blank = startPos;
    unsigned int front = startPos + 1;
    unsigned int behind = endPos;

    while(front < behind) {
        while(front < behind && arr[front] < mid) {
            front++;
        }
        while(front < behind && arr[behind] > mid) {
            behind--;
        }
        if(behind == startPos + 1) { //出现这种情况说明所有元素都比中间值大, 直接跳出
            break;
        }
        arr[blank] = arr[behind];
        arr[behind] = arr[front];
        blank = front;
    }
    arr[blank] = mid;

    quickSort(arr, startPos, behind - 1);
    quickSort(arr, behind, endPos);
}

int main() {
    vector<int> t = {10, 9, 8, 7, -111, 6, 5, 122123, 4, 3, 2, 1};
    quickSort(t, 0, t.size() - 1);
    return 0;
}