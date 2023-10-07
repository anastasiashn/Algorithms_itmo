#pragma once
#include <type_traits>
#include <stdexcept>


template <typename T>
class DynamicArray
{
public:

	class Iterator
	{
		friend class DynamicArray<T>;
	public:
		Iterator(DynamicArray<T>& arrRef, bool reverse = false);
		const T& get() const;
		void set(const T& value);
		void next();
		bool hasNext() const;
	private:
		int index;
		bool reverse;
		DynamicArray<T>& arrRef;
	};

	DynamicArray();
	DynamicArray(int size);
	~DynamicArray();
	int size();
	void insert(const T& value);
	void insert(int index, const T& value);
	void remove(int index);
	const T& operator[](int index) const;
	T& operator[](int index);
	Iterator iterator() {
		Iterator iterator(*this);
		return iterator;
	}
	const Iterator iterator() const {
		Iterator iterator(*this);
		return iterator;
	}
	Iterator reverseIterator() {
		Iterator iterator(*this, true);
		return iterator;
	}
	const Iterator reverseIterator() const {
		Iterator iterator(*this, true);
		return iterator;
	}
private:
	const int emptyConstructArraySize = 8;
	int realSize;
	int publicSize;
	T* arr;
	T* createArray(int size);
	void increaseSize();
	void increaseSize(int index, const T& value);
};
