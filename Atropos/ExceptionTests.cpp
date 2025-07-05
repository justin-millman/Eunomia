#include "stdafx.h"												// for UnitTesting framework
#include "CppUnitTest.h"										// for UnitTesting framework
#include "../Eunomia/Parser.h"									// for ParserException
#include "../Eunomia/Utility.h"									// for EunomiaException

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using std::strcmp;


namespace Atropos {
	TEST_CLASS(ExceptionTester) {
		public:
			TEST_METHOD(EunomiaException_GetMessage) {
				const char* str = "Help! I'm trapped in a EunomiaException!!! :(";
				EunomiaException ex{ str };
				Assert::IsTrue(strcmp(ex.what(), str) == 0);
			}

			TEST_METHOD(ParserException_GetMessage) {
				const char* str = "Help! I'm trapped in a ParseException!!! :(";
				ParseException ex{ str };
				Assert::IsTrue(strcmp(ex.what(), str) == 0);
			}
	};
}