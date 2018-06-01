#include <vector>

using std::vector;

int min(int &a, int &b) {
    if(a > b) {
        return b;
    } else {
        return a;
    }
}

int getMedianOfTwoSortedArray(vector<int> &arrA, vector<int> &arrB) {
    auto ptrA = arrA.begin();
    auto ptrB = arrB.begin();
    unsigned int count = 1;
    unsigned int halfSize = (arrA.size() + arrB.size()) >> 1;
    //复杂度 o((m+n)/2)
    while (count < halfSize) {
        if(*ptrA > *ptrB) {
            ptrB++;
        } else {
            ptrA++;
        }
        count++;
    }

    auto tempA = ptrA;
    auto tempB = ptrB;
    if(ptrA + 1 != arrA.end()) {
        tempA = ptrA + 1;
    }
    if(ptrB + 1 != arrB.end()) {
        tempB = ptrB + 1;
    }
    return min(*ptrA, *ptrB);
}

double _getMedianOfTwoSortedArray(vector<int> &arrA, vector<int> &arrB) {
    unsigned int midA = arrA.size() / 2;
    unsigned int midB = arrA.size() / 2;
    unsigned int leftA = 0;
    unsigned int leftB = 0;
    unsigned int rightA = arrA.size() - 1;
    unsigned int rightB = arrB.size() - 1;

    while(midA - leftA >= 2 && midB - leftB >= 2) {
        if(arrA[midA] > arrB[midB]) {
            leftB = midB;
            midB = (leftB + rightB) / 2;
        } else {
            leftA = midA;
            midA = (leftA + rightA) / 2;
        }
    }

    return min(arrA[midA], arrB[midB]);
}

int main() {

    vector<int> a = {2, 4, 6, 8, 10};
    vector<int> b = {1, 3, 5, 7, 9};


    vector<int> c = {1, 2, 3, 4, 5, 6};
    vector<int> d = {7, 8, 9, 10, 11, 12};

    vector<int> e = {2, 4, 6, 8, 20};
    vector<int> f = {11, 13, 15, 17, 19};

    int g = getMedianOfTwoSortedArray(a, b);
    int h = getMedianOfTwoSortedArray(c, d);
    int i = getMedianOfTwoSortedArray(e, f);

    int j = getMedianOfTwoSortedArray(a, b);
    int k = getMedianOfTwoSortedArray(c, d);
    int l = getMedianOfTwoSortedArray(e, f);

    return 0;
}
