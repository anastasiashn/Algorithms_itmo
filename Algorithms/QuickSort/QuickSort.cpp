#include "pch.h"
#include "QuickSort.h"
#include <utility> 


template<typename T, typename Compare>
void QuickSort::sort(T* first, T* last, Compare comp) {
	int length = getLength(first, last);
	if (length < 2) {
		return;
	}

	if (length <= 20) {
		insertionSort(first, last, comp);
		return;
	}

	quickSort(first, last, comp);
}

template<typename T, typename Compare>
static void QuickSort::quickSort(T* first, T* last, Compare comp) {
	int length = getLength(first, last);

	T pivot = getPivot(first, last, comp);

	int left = 0;
	int right = length - 1;
	while (left <= right) {
		while (comp(first[left], pivot))
			left++;
		while (comp(pivot, first[right]))
			right--;
		if (left <= right) {
			swap(&first[left], &first[right]);
			left++;
			right--;
		}
	}

	if (right > 0)
		sort(first, first + right, comp);
	if (length > left)
		sort(first + left, last, comp);
}

template<typename T, typename Compare>
void QuickSort::insertionSort(T* first, T* last, Compare comp) {
	int length = getLength(first, last);
	for (int step = 1; step < length; step++) {
		T key = std::move(first[step]);
		int j = step - 1;

		while (comp(key, first[j]) && j >= 0) {
			first[j + 1] = std::move(first[j]);
			--j;
		}
		first[j + 1] = std::move(key);
	}
}

template<typename T, typename Compare>
T QuickSort::getPivot(T* first, T* last, Compare comp) {
	const int length = 3;
	T array[length];
	array[0] = *first;
	array[1] = *(first + getLength(first, last) / 2);
	array[2] = *last;
	insertionSort(array, array + length - 1, comp);
	return array[1];
}

template<typename T>
void QuickSort::swap(T* first, T* second) {
	T temp = *first;
	*first = *second;
	*second = temp;
	return;
}

template<typename T>
int QuickSort::getLength(T* first, T* last) {
	return last - first + 1;
}