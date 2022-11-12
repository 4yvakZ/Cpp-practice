#pragma once
#include <iostream>

template<typename T>
void printArray(T* first, T* last) {
    while (first < last) {
        std::cout << *first << " ";
        first++;
    }
    std::cout << "\n";
}

template<typename T>
void moveSwap(T* a, T* b) {
    T tmp(std::move(*a));
    *a = std::move(*b);
    *b = std::move(tmp);
}

template<typename T, typename Compare>
void insertionSort(T* first, T* last, Compare comp) {
    for (T* p = first + 1; p < last; p++) {
        T* q = p - 1;
        T tmp = std::move(*p);
        while (q >= first && comp(tmp, *q)) {
            *(q + 1) = std::move(*q);
            q--;
        }
        *(q + 1) = std::move(tmp);
    }
}

template<typename T, typename Compare>
T* partition(T* first, T* last, Compare comp) {

    T* middle = first + (last - first) / 2;
    T pivot;
    last--;
    if (comp(*first, *last)) {//f < l
        if (comp(*middle, *first)) {//m < f
            pivot = *first;
        }
        else if (comp(*middle, *last)) {//m < l
            pivot = *middle;
        }
        else {
            pivot = *last;
        }
    }
    else {
        if (comp(*first, *middle)) {//f < m
            pivot = *first;
        }
        else if (comp(*last, *middle)) {//l < m
            pivot = *middle;
        }
        else {
            pivot = *last;
        }
    }
    last++;


    T* p = first, * q = last - 1;
    while (p < q) {
        while (comp(*p, pivot)) {
            p++;
        }
        while (comp(pivot, *q)) {
            q--;
        }
        if (p < q) {
            if (*q == *p) {
                p++;
            }
            else {
                moveSwap(p, q);
            }
        }
    }

    return q;
}

template<typename T, typename Compare>
void quickSort(T* first, T* last, Compare comp) {
    while (first < last) {
        //printArray(first, last);
        T* pivot = partition(first, last, comp);
        //printArray(first, last);
        //std::cout << "Pivot index: " << pivot - first << " value: " << *pivot << std::endl;
        if (pivot - first < last - pivot + 1) {
            quickSort(first, pivot, comp);
            first = pivot + 1;
        }
        else {
            quickSort(pivot + 1, last, comp);
            last = pivot;
        }
    }
}

constexpr int kAlgoritmChangeSize = 11;
template<typename T, typename Compare>
void sort(T* first, T* last, Compare comp) {
    while (last - first > kAlgoritmChangeSize) {
        T* pivot = partition(first, last, comp);
        if (pivot - first < last - pivot + 1) {
            sort(first, pivot, comp);
            first = pivot + 1;
        }
        else {
            sort(pivot + 1, last, comp);
            last = pivot;
        }
    }
    insertionSort(first, last, comp);
}
