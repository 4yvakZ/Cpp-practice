// Lab3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "sorts.h"

template<typename T>
void printArray(T* first, T* last) {
    while (first < last) {
        std::cout << *first << " ";
        first++;
    }
    std::cout << "\n";
}

constexpr int kArraySize = 6;
int main()
{
    int a[kArraySize] = { 9, 3, 4, 2, 1, 8 };
    
    printArray(a, a + kArraySize);
    insertionSort(a, a + kArraySize, [](int a, int b) { return a < b; });
    //quickSort(a, a + kArraySize, [](int a, int b) { return a < b; });
    printArray(a, a + kArraySize);    
}