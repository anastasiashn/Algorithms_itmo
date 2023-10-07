#pragma once
class QuickSort
{
public:
    template<typename T, typename Compare>
    static void sort(T* first, T* last, Compare comp);

private:
    template<typename T, typename Compare>
    static void insertionSort(T* first, T* last, Compare comp);

    template<typename T, typename Compare>
    static void quickSort(T* first, T* last, Compare comp);

    template<typename T, typename Compare>
    static T getPivot(T* first, T* last, Compare comp);

    template<typename T>
    static void swap(T* first, T* second);

    template<typename T>
    static int getLength(T* first, T* last);
};