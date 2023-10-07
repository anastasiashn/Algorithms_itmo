#include "pch.h"
#include "DynamicArray.h"

template <typename T>
DynamicArray<T>::DynamicArray() {
	arr = createArray(emptyConstructArraySize);
	realSize = emptyConstructArraySize;
	publicSize = 0;
}

template <typename T>
DynamicArray<T>::DynamicArray(int size) {
	arr = createArray(size);
	realSize = size;
	publicSize = size;
}

template <typename T>
DynamicArray<T>::~DynamicArray() {
	free(arr);
}

template <typename T>
int DynamicArray<T>::size() {
	return publicSize;
}

template <typename T>
void DynamicArray<T>::insert(const T& value) {
	if (publicSize == realSize) {
		increaseSize();
	}

	arr[publicSize] = value;
	publicSize++;
}

template <typename T>
void DynamicArray<T>::insert(int index, const T& value) {
	if (realSize == index) {
		increaseSize();
		arr[index] = value;
		publicSize++;
	}

	if (publicSize == realSize) {
		increaseSize(index, value);
		return;
	}
	
	for (int i = realSize - 2; i >= index; --i) {
		arr[i + 1] = arr[i];
	}
	arr[index] = value;
	publicSize++;
}

template <typename T>
void DynamicArray<T>::remove(int index) {
	for (int i = index; i < realSize - 1; ++i) {
		arr[i] = arr[i + 1];
	}
	if (std::is_destructible<T>::value) {
		arr[realSize - 1].~T();
	}
	publicSize--;
}

template <typename T>
const T& DynamicArray<T>::operator[](int index) const {
	if (index < 0 || index >= publicSize) {
		throw std::out_of_range("index out of range");
	}
	return arr[index];
}

template <typename T>
T& DynamicArray<T>::operator[](int index) {
	if (index < 0 || index >= publicSize) {
		throw std::out_of_range("index out of range");
	}
	return arr[index];
}

template <typename T>
void DynamicArray<T>::increaseSize() {
	T* oldArray = arr;
	realSize = (int)(realSize * 1.6);
	T* newArray = createArray(realSize);
	for (int i = 0; i < publicSize; ++i) {
		newArray[i] = arr[i];
	}
	arr = newArray;
	free(oldArray);
}

template <typename T>
T* DynamicArray<T>::createArray(int size) {
	return (T*)malloc(size * sizeof(T));
}

template <typename T>
void DynamicArray<T>::increaseSize(int index, const T& value) {
	T* oldArray = arr;
	publicSize++;
	realSize = (int)(realSize * 1.6);
	T* newArray = createArray(realSize);
	int supIndex = 0;
	for (int i = 0; i < publicSize; ++i) {
		if (i == index) {
			newArray[i + supIndex] = value;
			++supIndex;
		}
		newArray[i + supIndex] = arr[i];
	}
	arr = newArray;
	free(oldArray);
}

template<typename T>
inline DynamicArray<T>::Iterator::Iterator(DynamicArray<T>& arrRef, bool reverse) : arrRef(arrRef), reverse(reverse)
{
	if (reverse) {
		index = arrRef.size() - 1;
	}
	else {
		index = 0;
	}
}

template<typename T>
const T& DynamicArray<T>::Iterator::get() const
{
	return arrRef.arr[index];
}

template<typename T>
void DynamicArray<T>::Iterator::set(const T& value)
{
	arrRef.arr[index] = value;
}

template<typename T>
void DynamicArray<T>::Iterator::next()
{
	if (reverse) {
		index--;
	}
	else {
		index++;
	}
}

template<typename T>
bool DynamicArray<T>::Iterator::hasNext() const
{
	if (reverse) {
		return index > 0;
	}

	return index < arrRef.publicSize - 1;
}