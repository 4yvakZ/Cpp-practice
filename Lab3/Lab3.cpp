// Lab3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "sorts.h"
#include <Windows.h>

template<typename T>
void printArray(T* first, T* last) {
    while (first < last) {
        std::cout << *first << " ";
        first++;
    }
    std::cout << "\n";
}

constexpr int kArraySize = 5000;
constexpr int kNumberOfCycles = 1000;
constexpr int kMaxTestInsertionLength = 200;
int main()
{
    int a[kArraySize];
    LARGE_INTEGER StartingTime, EndingTime;
    LARGE_INTEGER frequancy;
    QueryPerformanceFrequency(&frequancy);
    double PCFreq = double(frequancy.QuadPart) / 1000;

    for (int algorithmChangeLength = 0; algorithmChangeLength < kMaxTestInsertionLength; algorithmChangeLength++) {
        std::cout.width(4);
        std::cout << algorithmChangeLength << " : ";
        QueryPerformanceCounter(&StartingTime);
        for (int cycle = 0; cycle < kNumberOfCycles; cycle++) {
            
            for (int i = 0; i < kArraySize; i++) {
                a[i] = kArraySize - i;
            }
            //QueryPerformanceCounter(&StartingTime);
            sort(a, a + kArraySize, [](int a, int b) { return a < b; }, 3);
        }
        QueryPerformanceCounter(&EndingTime);

        std::cout << (EndingTime.QuadPart - StartingTime.QuadPart) / PCFreq << "ms\n";
    }
}