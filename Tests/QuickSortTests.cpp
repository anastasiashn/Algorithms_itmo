#include "pch.h"
#include "CppUnitTest.h"
#include <tuple>
#include <algorithm>
#include "../Algorithms/QuickSort/QuickSort.h"
#include "../Algorithms/QuickSort/QuickSort.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

template < typename T >
void ArrayEquals(T* expected, T* actual, int size) {
	int index = 0;
	for (int index = 0; index < size; index++) {
		Assert::AreEqual(expected[index], actual[index]);
	}
}

std::tuple<int*, int*, int*> RandomArray(int length, bool descending) {
	int* arr = new int[length];
	int* sortedArr = new int[length];
	for (int i = 0; i < length; i++) {
		int randInt = rand() % 10000;
		arr[i] = randInt;
		sortedArr[i] = randInt;
	}
	if (descending) {
		std::sort(sortedArr, sortedArr + length, std::greater<int>());
	}
	else {
		std::sort(sortedArr, sortedArr + length);
	}
	return { arr, arr + length - 1, sortedArr };
}

namespace QuickSortTests
{
	TEST_CLASS(QuickSortTests)
	{
	public:
		TEST_METHOD(QuickSort_20Element)
		{
			QuickSortTests::TestAscending(20);
		}

		TEST_METHOD(QuickSort_200Element)
		{
			QuickSortTests::TestAscending(200);
		}

		TEST_METHOD(QuickSort_1000Element)
		{
			QuickSortTests::TestAscending(1000);
		}

		TEST_METHOD(QuickSort_Descending_20Element)
		{
			QuickSortTests::TestDescending(20);
		}

		TEST_METHOD(QuickSort_Descending_200Element)
		{
			QuickSortTests::TestDescending(200);
		}

		TEST_METHOD(QuickSort_Descending_1000Element)
		{
			QuickSortTests::TestDescending(1000);
		}

	private:
		static void TestDescending(int length) {
			int* first;
			int* last;
			int* sortedFirst;
			std::tie(first, last, sortedFirst) = RandomArray(length, true);
			QuickSort::sort(first, last, [](int a, int b) { return a > b; });
			ArrayEquals(sortedFirst, first, length);
			delete[] first;
			delete[] sortedFirst;
		}

		static void TestAscending(int length) {
			int* first;
			int* last;
			int* sortedFirst;
			std::tie(first, last, sortedFirst) = RandomArray(length, false);
			QuickSort::sort(first, last, [](int a, int b) { return a < b; });
			ArrayEquals(sortedFirst, first, length);
			delete[] first;
			delete[] sortedFirst;
		}
	};
}
