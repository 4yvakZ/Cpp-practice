// Lab3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include "sorts.h"
#include <Windows.h>

constexpr int kNumberOfCycles = 10000;
constexpr int kMaxArrayLength = 20;
int main()
{
    int c[10] = { 8, 3, 2, 3, 1, 4, 1, 9, 1, 10 };
    /*for (int i = 0; i < 10; i++) {
        c[i] = 10 - i;
    }*/


    sort(c, c + 10, [](int a, int b) { return a < b; });
    printArray(c, c + 10);

    //return 0;

    int a[kMaxArrayLength];
    LARGE_INTEGER StartingTime, EndingTime;
    LARGE_INTEGER frequancy;
    QueryPerformanceFrequency(&frequancy);
    double PCFreq = double(frequancy.QuadPart) / 1000;

    std::ofstream fout("quickSort.txt", std::ios_base::out | std::ios_base::trunc);
    fout << "array_len, time" << std::endl;

    for (int arrayLength = 1; arrayLength < kMaxArrayLength; arrayLength++) {
        std::cout.width(4);
        std::cout << arrayLength << " : ";
        QueryPerformanceCounter(&StartingTime);
        for (int cycle = 0; cycle < kNumberOfCycles; cycle++) {
            
            for (int i = 0; i < arrayLength; i++) {
                a[i] = arrayLength - i;
            }

            quickSort(a, a + arrayLength, [](int a, int b) { return a < b; });
        }
        QueryPerformanceCounter(&EndingTime);
        fout << arrayLength << "," << (EndingTime.QuadPart - StartingTime.QuadPart) / PCFreq << std::endl;
        std::cout << (EndingTime.QuadPart - StartingTime.QuadPart) / PCFreq << "ms\n";
    }
    fout.close();

    fout.open("insertionSort.txt", std::ios_base::out | std::ios_base::trunc);

    fout << "array_len, time" << std::endl;

    for (int arrayLength = 1; arrayLength < kMaxArrayLength; arrayLength++) {
        std::cout.width(4);
        std::cout << arrayLength << " : ";
        QueryPerformanceCounter(&StartingTime);
        for (int cycle = 0; cycle < kNumberOfCycles; cycle++) {

            for (int i = 0; i < arrayLength; i++) {
                a[i] = arrayLength - i;
            }

            insertionSort(a, a + arrayLength, [](int a, int b) { return a < b; });
        }
        QueryPerformanceCounter(&EndingTime);
        fout << arrayLength << "," << (EndingTime.QuadPart - StartingTime.QuadPart) / PCFreq << std::endl;
        std::cout << (EndingTime.QuadPart - StartingTime.QuadPart) / PCFreq << "ms\n";
    }
    fout.close();
}