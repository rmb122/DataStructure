#include <iostream>
#include <vector>

using std::vector;

unsigned int& max(vector<int> &arr, unsigned int &a, unsigned int &b) {
    if(arr[a] > arr[b]) {
        return a;
    }
    return b;
}

void swap(vector<int> &arr, unsigned int a, unsigned int b) {
    if(arr[a] == arr[b]) {
        return;
    }
    arr[a] ^= arr[b];
    arr[b] ^= arr[a];
    arr[a] ^= arr[b];
}

void heapify(vector<int> &arr, unsigned int currPos, unsigned int endPos) {
    unsigned int rightNodePos = 2 * (currPos + 1); //得到右节点位置
    unsigned int leftNodePos = rightNodePos - 1; //得到左节点位置
    unsigned int maxNodePos;

    if(rightNodePos < endPos) { //判断右节点是否存在
        maxNodePos = max(arr, rightNodePos, leftNodePos);
        if(arr[currPos] < arr[maxNodePos]) { //根节点比两个子节点中最大的小, 则与之交换
            swap(arr, currPos, maxNodePos);
            heapify(arr, maxNodePos, endPos); //对调换过后的节点再次进行调整, 防止使堆再次混乱
        }
    } else {
        if(leftNodePos < endPos) { //判断左节点是否存在
            if(arr[currPos] < arr[leftNodePos]) {
                swap(arr, currPos, leftNodePos);
            }
        }
    }
}

void makeBigEndianHeap(vector<int> &arr) {
    for(unsigned int i = arr.size() / 2 + 1; i >= 1; i--) { //如果使用 i >= 0, i = 0 时减 1 得到 UINT32_MAX > 0, 导致段错误并且死循环
        heapify(arr, i - 1, arr.size());
    }
}

void heapSort(vector<int> &arr) {
    makeBigEndianHeap(arr);

    for(unsigned int i  = arr.size(); i >= 1; i--) {
        swap(arr, i - 1, 0); //交换顶端元素与未固定元素的最后一个
        heapify(arr, 0, i - 1); //重新调整堆
    }
}