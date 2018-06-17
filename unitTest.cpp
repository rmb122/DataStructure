#include "shellSort.cpp"
#include <random>
#include <cassert>

std::vector<int> getRandomArr(unsigned int size, int downLimit, int uplimit) {
    std::random_device engine;
    std::uniform_int_distribution<int> num(downLimit, uplimit);
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

    for(unsigned int i = 0; i < 1000; i++) {
        vector<int> test = getRandomArr(1000, INT32_MIN, INT32_MAX);
        shellSort(test);
        assert(isOrdered(test, 0, test.size()));
    }

    /*
     std::vector<int> test = getRandomArr(100, -9999, 99999);
     tree t;
     for(int num : test) {
         t.insert(num);
     }
     std::cout << std::endl;
     t.print();
     */
    return 0;
}