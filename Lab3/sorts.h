#pragma once
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
    T* pivot;
    last--;
    if (comp(*first, *last)) {//f < l
        if (comp(*middle, *first)) {//m < f
            pivot = first;
        }
        else if (comp(*middle, *last)) {//m < l
            pivot = middle;
        }
        else {
            pivot = last;
        }
    }
    else {
        if (comp(*first, *middle)) {//f < m
            pivot = first;
        }
        else if (comp(*last, *middle)) {//l < m
            pivot = middle;
        }
        else {
            pivot = last;
        }
    }
    last++;

    //������ ����� �� ��� �����
    int shift = -1;
    for (T* p = first; p < last; p++) {
        if (!comp(*pivot, *p)) {//p <= pivot
            shift++;
        }
    }

    moveSwap(pivot, first + shift);
    pivot = first + shift;

    //����������� ��������� �������� �� ����������
    T* p = first, * q = last - 1;
    while (p < pivot && q > pivot) {
        while (comp(*p, *pivot)) {
            p++;
        }
        while (comp(*pivot, *q)) {
            q--;
        }
        if (p < pivot && q > pivot) {
            moveSwap(p, q);
        }
    }

    return pivot;
}

template<typename T, typename Compare>
void quickSort(T* first, T* last, Compare comp) {
    while (first < last) {
        T* pivot = partition(first, last, comp);
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


