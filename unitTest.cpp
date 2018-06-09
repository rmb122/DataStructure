#include "./堆排序Ex.cpp"
#include <random>
#include <cassert>

std::vector<int> getRandomArr(unsigned int size) {
    std::default_random_engine engine;
    std::uniform_int_distribution<int> num(INT32_MIN, INT32_MAX);
    std::vector<int> result;
    for(unsigned int i = 0 ; i < size; i++) {
        result.push_back(num(engine));
    }
    return result;
}

bool isOrdered(std::vector<int> &arr, unsigned int startPos, unsigned int endPos) {
    for(unsigned int i = startPos + 1; i < endPos ; i++) {
        if(arr[i] < arr[i - 1]) {
            return false;
        }
    }
    return true;
}

int main() {
    for(unsigned int i = 0; i < 50; i++) {
        vector<int> test = getRandomArr(100);
        heapSort(test, 1, test.size() - 5);
        assert(isOrdered(test, 1, test.size() - 5));
    }
    return 0;
}