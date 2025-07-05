#include "stdafx.h"												// for UnitTesting framework
#include "CppUnitTest.h"										// for UnitTesting framework
#include "../Eunomia/Assignment.h"								// for Assignment

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Atropos {
	TEST_CLASS(AssignmentTester) {
		public:
			TEST_METHOD(Assignment_Construct) {
				const size_t num = 10;
				Assignment assign{ num };

				Assert::IsTrue(assign.size() == num);
				for (int i = 0; i < num; ++i) {
					Assert::IsTrue(!assign.getAssignedValue(i));
				}
				Assert::IsTrue(*assign.falseBegin() == 0);
				Assert::IsTrue(assign.trueBegin() == assign.trueEnd());
			}

			TEST_METHOD(Assignment_ValueSetters) {
				const size_t num = 300;
				Assignment assign{ num };
				
				assign.setTrue(17);
				assign.setTrue(0);
				assign.setTrue(num - 1);
				assign.setTrue(212);
				assign.setTrue(183);

				Assert::IsTrue(assign.getAssignedValue(17));
				Assert::IsTrue(assign.getAssignedValue(0));
				Assert::IsTrue(assign.getAssignedValue(num - 1));
				Assert::IsTrue(assign.getAssignedValue(212));
				Assert::IsTrue(assign.getAssignedValue(183));

				assign.setFalse(212);
				assign.setFalse(0);
				assign.setFalse(num - 1);
				Assert::IsTrue(!assign.getAssignedValue(212));
				Assert::IsTrue(!assign.getAssignedValue(0));
				Assert::IsTrue(!assign.getAssignedValue(num - 1));
			}

			TEST_METHOD(Assignment_TrueIterate) {
				const size_t num = 100;
				Assignment assign{ num };

				for (size_t i = 0; i < num; i += 3) {
					assign.setTrue(i);
				}

				int count = 0;
				for (auto iter = assign.trueBegin(); iter != assign.trueEnd(); ++iter) {
					++count;
					Assert::IsTrue(assign.getAssignedValue(*iter));
				}
				Assert::IsTrue(count == 34);
			}

			TEST_METHOD(Assignment_FalseIterate) {
				const size_t num = 100;
				Assignment assign{ num };

				for (size_t i = 0; i < num; i += 7) {
					assign.setTrue(i);
				}

				int count = 0;
				for (auto iter = assign.falseBegin(); iter != assign.falseEnd(); ++iter) {
					++count;
					Assert::IsTrue(!assign.getAssignedValue(*iter));
				}
				Assert::IsTrue(count == 85);
			}
	};
}