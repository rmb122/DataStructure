#include <iostream>
#include <vector>

void swap(int first, int second, int* array) {
    if(first == second) {
        return;
    }
    array[first] ^= array[second];
    array[second] ^= array[first];
    array[first] ^= array[second];
}

void reverse(int start, int end, int* array) {
    while(start < end) {
        swap(start, end, array);
        start++;
        end--;
    }
}

void shift(int val, int size, int* array) {
    reverse(0, val - 1, array);
    reverse(val, size - 1, array);
    reverse(0, size - 1, array);
}

int main() {
    int values[100];
    for(int i = 0; i < 100; i++) {
        values[i] = i;
    }
    std::cout << "Original array: " << std::endl;
    for(int i = 0; i < 100; i++) {
        std::cout << values[i] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;

    shift(99, 100, values);
    std::cout << "Shifted(99) array: " << std::endl;
    for(int i = 0; i < 100; i++) {
        std::cout << values[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}

