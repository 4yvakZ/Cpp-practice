#include "pch.h"
#include "CppUnitTest.h"
#include "../Array.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(simpleInt)
		{
			Array<int> a;
			Assert::AreEqual(0, a.size());
			for (int i = 0; i < 10; ++i) {
				a.insert(i + 1);
			}
			Assert::AreEqual(10, a.size());
			for (int i = 0; i < a.size(); ++i) {
				a[i] *= 2;
				Assert::AreEqual((i + 1) * 2, a[i]);
			}
		}

		TEST_METHOD(string) {
			Array<std::string> a;
			for (int i = 0; i < 10; i++) {
				Assert::AreEqual(i, a.insert("kek"));
			}
			Assert::AreEqual(10, a.size());
			a.remove(0);
			a.remove(0);
			Assert::AreEqual(8, a.size());    
			for (auto it = a.iterator(); it.hasNext(); it.next())
				std::cout << it.get() << std::endl;
		}

		TEST_METHOD(emptyArray) {
			Array<int> a;
			for (int i = 0; i < 10; i++) {
				Assert::AreEqual(i, a.insert(i+1));
			}
			Assert::AreEqual(10, a.size());
			for (int i = 0; i < 10; i++) {
				a.remove(1);
			}
			Assert::AreEqual(1, a.size());
			for (int i = 0; i < 10; i++) {
				a.remove(0);
			}
			Assert::AreEqual(0, a.size());
		}
		
		TEST_METHOD(outOfBounds) {
			Array<int> a;
			for (int i = 0; i < 10; i++) {
				Assert::AreEqual(i, a.insert(i + 1));
			}
			Assert::AreEqual(10, a.size());
			Assert::AreEqual(-1, a.insert(100, 1));
			Assert::AreEqual(10, a.size());
			a[-1];
		}
	};
}
