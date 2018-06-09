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

void heapify(vector<int> &arr, unsigned int currPos, unsigned int endPos, unsigned int &startPos) {
    unsigned int rightNodePos = 2 * (currPos - startPos + 1) + startPos; //得到右节点位置
    unsigned int leftNodePos = rightNodePos - 1; //得到左节点位置
    unsigned int maxNodePos;

    if(rightNodePos < endPos) { //判断右节点是否存在
        maxNodePos = max(arr, rightNodePos, leftNodePos);
        if(arr[currPos] < arr[maxNodePos]) { //根节点比两个子节点中最大的小, 则与之交换
            swap(arr, currPos, maxNodePos);
            heapify(arr, maxNodePos, endPos, startPos); //对调换过后的节点再次进行调整, 防止使堆再次混乱
        }
    } else {
        if(leftNodePos < endPos) { //判断左节点是否存在
            if(arr[currPos] < arr[leftNodePos]) {
                swap(arr, currPos, leftNodePos);
            }
        }
    }
}

void makeBigEndianHeap(vector<int> &arr, unsigned int startPos, unsigned int endPos) {
    for(unsigned int i = (endPos - startPos) / 2 + 1; i >= 1; i--) { //当 startPos = 0 时, 则循环条件是 i >= 0, i = 0 时减 1 使得 i = UINT32_MAX, 导致段错误并且死循环
        heapify(arr, i - 1, endPos, startPos);
    }
}

void heapSort(vector<int> &arr, unsigned int startPos, unsigned int endPos) { //增加任意起点和终点功能
    makeBigEndianHeap(arr, startPos, endPos);

    for(unsigned int i = endPos; i > startPos + 1; i--) {
        swap(arr, i - 1, startPos); //交换顶端元素与未固定元素的最后一个
        heapify(arr, startPos, i - 1, startPos); //重新调整堆
    }
}