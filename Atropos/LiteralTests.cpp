#include "stdafx.h"												// for UnitTesting framework
#include "CppUnitTest.h"										// for UnitTesting framework
#include "../Eunomia/Assignment.h"								// for Assignment
#include "../Eunomia/Literal.h"									// for Literal
#include "../Eunomia/Utility.h"									// for constant char symbols
#include <sstream>												// for stringstream
#include <string>												// for string

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using std::string;
using std::stringstream;


namespace Atropos {
	TEST_CLASS(LiteralTester) {
		public:
			TEST_METHOD(Literal_Construct) {
			const string str = "A";
			Literal literal{ 0, str };

			stringstream sstream{};
			sstream << literal;
			Assert::IsTrue(sstream.str() == str);
		}

			TEST_METHOD(Literal_ConstructNegated) {
				const string str = "Sayonara";
				Literal literal{ 0, str, true };

				stringstream sstream{};
				sstream << literal;
				Assert::IsTrue(sstream.str() == kNotSymbol + str);
			}

			TEST_METHOD(Literal_Negate) {
				const string str = "Caballo";
				Literal literal{ 0, str };
				literal.negate();

				stringstream sstream{};
				sstream << literal;
				Assert::IsTrue(sstream.str() == kNotSymbol + str);
			}

			TEST_METHOD(Literal_DoubleNegate) {
				const string str = "Pancreas";
				Literal literal{ 0, str };
				literal.negate();
				literal.negate();

				stringstream sstream{};
				sstream << literal;
				Assert::IsTrue(sstream.str() == str);
			}

			TEST_METHOD(Literal_Evaluate) {
				Literal literal{ 0, "Tortellini" };
				Assignment assign{ 1 };
				assign.setTrue(0);

				Assert::IsTrue(literal.evaluate(assign));

				literal.negate();
				Assert::IsTrue(!literal.evaluate(assign));

				assign.setFalse(0);
				Assert::IsTrue(literal.evaluate(assign));

				literal.negate();
				Assert::IsTrue(!literal.evaluate(assign));
			}
	};
}