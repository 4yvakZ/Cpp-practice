#include "pch.h"
#include "CppUnitTest.h"
#include "../sorts.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		TEST_METHOD(quickSortTest)
		{
			constexpr int n = 10;
			int a[n];
			for (int i = 0; i < n; i++) {
				a[i] = n - i;
			}
			insertionSort(a, a + n, [](int a, int b) { return a < b; });
			for (int i = 0; i < n; i++) {
				Assert::AreEqual(i + 1, a[i]);
			}
		}

		TEST_METHOD(quickSortTestOneElem)
		{
			constexpr int n = 1;
			int a[n];
			for (int i = 0; i < n; i++) {
				a[i] = n - i;
			}
			insertionSort(a, a + n, [](int a, int b) { return a < b; });
			for (int i = 0; i < n; i++) {
				Assert::AreEqual(i + 1, a[i]);
			}
		}
		TEST_METHOD(insertionSortTest)
		{
			constexpr int n = 10;
			int a[n];
			for (int i = 0; i < n; i++) {
				a[i] = n - i;
			}
			insertionSort(a, a + n, [](int a, int b) { return a < b; });
			for (int i = 0; i < n; i++) {
				Assert::AreEqual(i + 1, a[i]);
			}
		}

		TEST_METHOD(insertionSortTestOneElem)
		{
			constexpr int n = 1;
			int a[n];
			for (int i = 0; i < n; i++) {
				a[i] = n - i;
			}
			insertionSort(a, a + n, [](int a, int b) { return a < b; });
			for (int i = 0; i < n; i++) {
				Assert::AreEqual(i + 1, a[i]);
			}
		}
		TEST_METHOD(sortTest)
		{
			constexpr int n = 10;
			int a[n];
			for (int i = 0; i < n; i++) {
				a[i] = n - i;
			}
			sort(a, a + n, [](int a, int b) { return a < b; });
			for (int i = 0; i < n; i++) {
				Assert::AreEqual(i + 1, a[i]);
			}
		}

		TEST_METHOD(sortTestOneElem)
		{
			constexpr int n = 1;
			int a[n];
			for (int i = 0; i < n; i++) {
				a[i] = n - i;
			}
			sort(a, a + n, [](int a, int b) { return a < b; });
			for (int i = 0; i < n; i++) {
				Assert::AreEqual(i + 1, a[i]);
			}
		}
	};
}
