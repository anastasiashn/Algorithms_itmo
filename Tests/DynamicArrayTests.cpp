#include "pch.h"
#include "CppUnitTest.h"
#include "../Algorithms/DynamicArray/DynamicArray.h"
#include "../Algorithms/DynamicArray/DynamicArray.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DynamicArrayTests
{
	TEST_CLASS(DynamicArrayTests) {
	public:
		TEST_METHOD(TestSizeEmptyConstructor)
		{
			DynamicArray<int> arr;
			Assert::AreEqual(0, arr.size());
		}

		TEST_METHOD(TestSize)
		{
			DynamicArray<int> arr(17);
			Assert::AreEqual(17, arr.size());
		}

		TEST_METHOD(TestInsertBySize)
		{
			DynamicArray<int> arr;

			arr.insert(10);

			Assert::AreEqual(1, arr.size());
		}

		TEST_METHOD(TestRemoveBySize)
		{
			DynamicArray<int> arr(2);

			arr.remove(0);

			Assert::AreEqual(1, arr.size());
		}

		TEST_METHOD(TestInsert)
		{
			DynamicArray<int> arr;

			arr.insert(10);
			arr.insert(20);

			Assert::AreEqual(10, arr[0]);
			Assert::AreEqual(20, arr[1]);
		}

		TEST_METHOD(TestInsertToIndex)
		{
			DynamicArray<int> arr;
			arr.insert(10);
			arr.insert(20);

			arr.insert(1, 30);

			Assert::AreEqual(10, arr[0]);
			Assert::AreEqual(30, arr[1]);
			Assert::AreEqual(20, arr[2]);
		}

		TEST_METHOD(TestRemove)
		{
			DynamicArray<int> arr;
			arr.insert(10);
			arr.insert(20);
			arr.insert(30);

			arr.remove(1);

			Assert::AreEqual(10, arr[0]);
			Assert::AreEqual(30, arr[1]);
		}

		TEST_METHOD(TestIteratorHasNextWhenEmptyArray)
		{
			DynamicArray<int> arr;
			DynamicArray<int>::Iterator iterator = arr.iterator();

			Assert::AreEqual(false, iterator.hasNext());
		}

		TEST_METHOD(TestReverseIteratorHasNextWhenEmptyArray)
		{
			DynamicArray<int> arr;
			DynamicArray<int>::Iterator iterator = arr.reverseIterator();

			Assert::AreEqual(false, iterator.hasNext());
		}

		TEST_METHOD(TestIteratorHasNextWhenOneElement)
		{
			DynamicArray<int> arr;
			arr.insert(10);
			DynamicArray<int>::Iterator iterator = arr.iterator();

			Assert::AreEqual(false, iterator.hasNext());
		}

		TEST_METHOD(TestReverseIteratorHasNextWhenOneElement)
		{
			DynamicArray<int> arr;
			arr.insert(10);
			DynamicArray<int>::Iterator iterator = arr.reverseIterator();

			Assert::AreEqual(false, iterator.hasNext());
		}

		TEST_METHOD(TestIteratorHasNextWhenTwoElement)
		{
			DynamicArray<int> arr;
			arr.insert(10);
			arr.insert(20);
			DynamicArray<int>::Iterator iterator = arr.iterator();

			Assert::AreEqual(true, iterator.hasNext());
			iterator.next();
			Assert::AreEqual(false, iterator.hasNext());
		}

		TEST_METHOD(TestReverseIteratorHasNextWhenTwoElement)
		{
			DynamicArray<int> arr;
			arr.insert(10);
			arr.insert(20);
			DynamicArray<int>::Iterator iterator = arr.reverseIterator();

			Assert::AreEqual(true, iterator.hasNext());
			iterator.next();
			Assert::AreEqual(false, iterator.hasNext());
		}
		
		TEST_METHOD(TestIteratorGet)
		{
			DynamicArray<int> arr;
			for (int index = 0; index < 30; ++index) {
				arr.insert(index);
			}
			
			DynamicArray<int>::Iterator iterator = arr.iterator();

			for (int index = 0; index < 30; ++index) {
				Assert::AreEqual(arr[index], iterator.get());
				iterator.next();
			}
		}

		TEST_METHOD(TestReverseIteratorGet)
		{
			DynamicArray<int> arr;
			for (int index = 0; index < 30; ++index) {
				arr.insert(index);
			}

			DynamicArray<int>::Iterator iterator = arr.reverseIterator();
			for (int index = 29; index >= 0; --index) {
				Assert::AreEqual(arr[index], iterator.get());
				iterator.next();
			}
		}

		TEST_METHOD(TestIteratorSet)
		{
			DynamicArray<int> arr;
			arr.insert(10);
			arr.insert(20);
			DynamicArray<int>::Iterator iterator = arr.iterator();
			iterator.set(100);
			iterator.next();
			iterator.set(200);

			Assert::AreEqual(100, arr[0]);
			Assert::AreEqual(200, arr[1]);
		}

		TEST_METHOD(TestReverseIteratorSet)
		{
			DynamicArray<int> arr;
			arr.insert(10);
			arr.insert(20);
			DynamicArray<int>::Iterator iterator = arr.reverseIterator();
			iterator.set(100);
			iterator.next();
			iterator.set(200);

			Assert::AreEqual(200, arr[0]);
			Assert::AreEqual(100, arr[1]);
		}
	};
}