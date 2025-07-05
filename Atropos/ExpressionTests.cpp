#include "stdafx.h"												// for UnitTesting framework
#include "CppUnitTest.h"										// for UnitTesting framework
#include "../Eunomia/Clause.h"									// for Clause
#include "../Eunomia/Expression.h"								// for Expression
#include "../Eunomia/Literal.h"									// for Literal
#include "../Eunomia/Utility.h"									// for Operator_e
#include <memory>												// for unique_ptr, make_unique
#include <sstream>												// for stringstream
#include <string>												// for string
#include <utility>												// for move

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using std::string;
using std::stringstream;
using std::unique_ptr; using std::make_unique;
using std::move;


namespace Atropos {
	TEST_CLASS(ExpressionTester) {
		public:
			TEST_METHOD(Expression_PrintLiteral) {
				string str = "Heart";
				unique_ptr<Expression> expr = make_unique<Literal>(0, str, true);

				stringstream sstream{};
				sstream << *expr;

				string result = kNotSymbol + str;
				Assert::IsTrue(sstream.str() == result);
			}

			TEST_METHOD(Expression_PrintClause) {
				string str1 = "Radius";
				string str2 = "Ulna";
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> expr = make_unique<Clause>(move(literal1), move(literal2), Operator_e::And);

				stringstream sstream{};
				sstream << *expr;

				string result = '(' + str1 + ' ' + kAndSymbol + ' ' + str2 + ')';
				Assert::IsTrue(sstream.str() == result);
			}
	};
}