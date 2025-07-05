#include "stdafx.h"												// for UnitTesting framework
#include "CppUnitTest.h"										// for UnitTesting framework
#include "../Eunomia/Category.h"								// for Category
#include <set>													// for set
#include <string>												// for string
#include <vector>												// for vector

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using std::string;
using std::vector; using std::set;


namespace Atropos {
	TEST_CLASS(CategoryTester) {
		public:
			TEST_METHOD(Category_ConstructVector) {
				vector<string> items{ "Banana", "Guava", "Dragonfruit", "Orange",
					"Grapefruit", "Cantaloupe", "Green Grape", "Cherry", "Watermelon" };

				const string name = "Fruits";
				Category category{ name, items.cbegin(), items.cend() };

				Assert::IsTrue(category.size() == items.size());
				Assert::IsTrue(category.getName() == name);
			}

			TEST_METHOD(Category_ConstructArray) {
				string items[]{ "Madrid", "Toledo", "Barcelona", "Ceuta", "Sevilla",
					"Valencia", "Bilbao", "Granada" };

				size_t num = sizeof(items) / sizeof(string);
				const string name = "Spanish Cities";
				Category category{ name, items, items + num };

				Assert::IsTrue(category.size() == num);
				Assert::IsTrue(category.getName() == name);
			}

			TEST_METHOD(Category_ConstructSet) {
				set<string> items{ "Leprechaun", "Dublin", "Blarney Stone", "Potatoes",
					"St. Patrick", "Eire", "Giant's Causeway", "Finn MacCool", "Ulster",
					"Catholicism", "Shamrock" };

				const string name = "Irish Things";
				Category category{ name, items.cbegin(), items.cend() };

				Assert::IsTrue(category.size() == items.size());
				Assert::IsTrue(category.getName() == name);
			}

			TEST_METHOD(Category_Index) {
				vector<string> items{ "Bonobo", "Gorilla", "Chimpanzee", "Mandrill",
					"Capuchin" };
				Category category{ "Primates", items.cbegin(), items.cend() };

				size_t size = items.size();
				for (size_t i = 0; i < size; ++i) {
					Assert::IsTrue(items[i] == category[i]);
				}
			}

			TEST_METHOD(Category_Iterate) {
				vector<string> items{ "string", "bool", "int", "long long",
				 "char", "unsigned", "vector", "const char*", "double", "float" };
				Category category{ "Data Types", items.cbegin(), items.cend() };

				size_t count = 0;
				for (const auto& item : category) {
					Assert::IsTrue(item == items[count++]);
				}
				Assert::IsTrue(count == items.size());
				Assert::IsTrue(count == category.size());
			}
	};
}