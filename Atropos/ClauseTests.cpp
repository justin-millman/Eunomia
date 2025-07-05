#include "stdafx.h"												// for UnitTesting framework
#include "CppUnitTest.h"										// for UnitTesting framework
#include "../Eunomia/Assignment.h"								// for Assignment
#include "../Eunomia/Clause.h"									// for Clause
#include "../Eunomia/Expression.h"								// for Expression
#include "../Eunomia/Literal.h"									// for Literal
#include "../Eunomia/Utility.h"									// for Operator_e, constant char symbols
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
	TEST_CLASS(ClauseTester) {
		public:
			TEST_METHOD(Clause_AndLiteralsConstruct) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				Clause clause{ move(literal1), move(literal2), Operator_e::And };

				stringstream sstream;
				sstream << clause;

				stringstream sresult;
				sresult << '(' << str1 << ' ' << kAndSymbol << ' ' << str2 << ')';

				Assert::IsTrue(sstream.str() == sresult.str());
			}

			TEST_METHOD(Clause_AndLiteralsNegate) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				Clause clause{ move(literal1), move(literal2), Operator_e::And };
				clause.negate();

				stringstream sstream{};
				sstream << clause;

				stringstream sresult{};
				sresult << '(' << kNotSymbol << str1 << ' ' << kOrSymbol << ' ' << kNotSymbol << str2 << ')';

				Assert::IsTrue(sstream.str() == sresult.str());
			}

			TEST_METHOD(Clause_AndLiteralsEvaluate) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				Clause clause{ move(literal1), move(literal2), Operator_e::And };

				Assignment assign{ 2 };
				Assert::IsTrue(!clause.evaluate(assign));

				assign.setTrue(0);
				Assert::IsTrue(!clause.evaluate(assign));

				assign.setTrue(1);
				Assert::IsTrue(clause.evaluate(assign));

				assign.setFalse(0);
				Assert::IsTrue(!clause.evaluate(assign));
			}

			TEST_METHOD(Clause_AndLiteralsEvaluateNegated) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				Clause clause{ move(literal1), move(literal2), Operator_e::And };
				clause.negate();

				Assignment assign{ 2 };
				Assert::IsTrue(clause.evaluate(assign));

				assign.setTrue(0);
				Assert::IsTrue(clause.evaluate(assign));

				assign.setTrue(1);
				Assert::IsTrue(!clause.evaluate(assign));

				assign.setFalse(0);
				Assert::IsTrue(clause.evaluate(assign));
			}

			TEST_METHOD(Clause_AndClausesConstruct) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1, true);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> literal4 = make_unique<Literal>(3, str4);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::Or);
				unique_ptr<Expression> clause2 = make_unique<Clause>(move(literal3), move(literal4), Operator_e::Xor);
				Clause clause3{ move(clause1), move(clause2), Operator_e::And };

				stringstream sstream{};
				sstream << clause3;

				stringstream sresult{};
				sresult << '('
					<< '(' << kNotSymbol << str1 << ' ' << kOrSymbol << ' ' << str2 << ')'
					<< ' ' << kAndSymbol << ' '
					<< '(' << str3 << ' ' << kXorSymbol << ' ' << str4 << ')'
					<< ')';

				Assert::IsTrue(sstream.str() == sresult.str());
			}

			TEST_METHOD(Clause_AndClausesNegate) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1, true);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> literal4 = make_unique<Literal>(3, str4);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::Or);
				unique_ptr<Expression> clause2 = make_unique<Clause>(move(literal3), move(literal4), Operator_e::Xor);
				Clause clause3{ move(clause1), move(clause2), Operator_e::And };
				clause3.negate();

				stringstream sstream{};
				sstream << clause3;

				stringstream sresult{};
				sresult << '('
					<< '(' << str1 << ' ' << kAndSymbol << ' ' << kNotSymbol << str2 << ')'
					<< ' ' << kOrSymbol << ' '
					<< '('
					<< '(' << str3 << ' ' << kAndSymbol << ' ' << str4 << ')'
					<< ' ' << kOrSymbol << ' '
					<< '(' << kNotSymbol << str3 << ' ' << kAndSymbol << ' ' << kNotSymbol << str4 << ')'
					<< ')'
					<< ')';

				Assert::IsTrue(sstream.str() == sresult.str());
			}

			TEST_METHOD(Clause_AndClausesEvaluate) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1, true);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> literal4 = make_unique<Literal>(3, str4);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::Or);
				unique_ptr<Expression> clause2 = make_unique<Clause>(move(literal3), move(literal4), Operator_e::Xor);
				Clause clause3{ move(clause1), move(clause2), Operator_e::And };

				Assignment assign{ 4 };
				assign.setTrue(0);
				Assert::IsTrue(!clause3.evaluate(assign));

				assign.setFalse(0);
				Assert::IsTrue(!clause3.evaluate(assign));

				assign.setTrue(3);
				Assert::IsTrue(clause3.evaluate(assign));

				assign.setTrue(0);
				Assert::IsTrue(!clause3.evaluate(assign));
			}

			TEST_METHOD(Clause_AndClausesEvaluateNegated) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1, true);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> literal4 = make_unique<Literal>(3, str4);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::Or);
				unique_ptr<Expression> clause2 = make_unique<Clause>(move(literal3), move(literal4), Operator_e::Xor);
				Clause clause3{ move(clause1), move(clause2), Operator_e::And };
				clause3.negate();

				Assignment assign{ 4 };
				assign.setTrue(0);
				Assert::IsTrue(clause3.evaluate(assign));

				assign.setFalse(0);
				Assert::IsTrue(clause3.evaluate(assign));

				assign.setTrue(3);
				Assert::IsTrue(!clause3.evaluate(assign));

				assign.setTrue(0);
				Assert::IsTrue(clause3.evaluate(assign));
		}

			TEST_METHOD(Clause_AndMixedConstruct) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::Or);
				Clause clause2{ move(clause1), move(literal3), Operator_e::And };

				stringstream sstream{};
				sstream << clause2;

				stringstream sresult{};
				sresult << '('
					<< '(' << str1 << ' ' << kOrSymbol << ' ' << str2 << ')'
					<< ' ' << kAndSymbol << ' '
					<< str3
					<< ')';

				Assert::IsTrue(sstream.str() == sresult.str());
			}

			TEST_METHOD(Clause_AndMixedNegate) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::Or);
				Clause clause2{ move(clause1), move(literal3), Operator_e::And };
				clause2.negate();

				stringstream sstream{};
				sstream << clause2;

				stringstream sresult{};
				sresult << '('
					<< '(' << kNotSymbol << str1 << ' ' << kAndSymbol << ' ' << kNotSymbol << str2 << ')'
					<< ' ' << kOrSymbol << ' '
					<< kNotSymbol << str3
					<< ')';

				Assert::IsTrue(sstream.str() == sresult.str());
			}

			TEST_METHOD(Clause_AndMixedEvaluate) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::Or);
				Clause clause2{ move(clause1), move(literal3), Operator_e::And };

				Assignment assign{ 3 };
				Assert::IsTrue(!clause2.evaluate(assign));

				assign.setTrue(0);
				Assert::IsTrue(!clause2.evaluate(assign));

				assign.setTrue(2);
				Assert::IsTrue(clause2.evaluate(assign));

				assign.setFalse(0);
				Assert::IsTrue(!clause2.evaluate(assign));
			}

			TEST_METHOD(Clause_AndMixedEvaluateNegated) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::Or);
				Clause clause2{ move(clause1), move(literal3), Operator_e::And };
				clause2.negate();

				Assignment assign{ 3 };
				Assert::IsTrue(clause2.evaluate(assign));

				assign.setTrue(0);
				Assert::IsTrue(clause2.evaluate(assign));

				assign.setTrue(2);
				Assert::IsTrue(!clause2.evaluate(assign));

				assign.setFalse(0);
				Assert::IsTrue(clause2.evaluate(assign));
			}

			TEST_METHOD(Clause_OrLiteralsConstruct) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				Clause clause{ move(literal1), move(literal2), Operator_e::Or };

				stringstream sstream{};
				sstream << clause;

				stringstream sresult{};
				sresult << '(' << str1 << ' ' << kOrSymbol << ' ' << str2 << ')';

				Assert::IsTrue(sstream.str() == sresult.str());
			}

			TEST_METHOD(Clause_OrLiteralsNegate) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				Clause clause{ move(literal1), move(literal2), Operator_e::Or };
				clause.negate();

				stringstream sstream{};
				sstream << clause;

				stringstream sresult{};
				sresult << '(' << kNotSymbol << str1 << ' ' << kAndSymbol << ' ' << kNotSymbol << str2 << ')';

				Assert::IsTrue(sstream.str() == sresult.str());
			}

			TEST_METHOD(Clause_OrLiteralsEvaluate) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				Clause clause{ move(literal1), move(literal2), Operator_e::Or };

				Assignment assign{ 2 };
				Assert::IsTrue(!clause.evaluate(assign));

				assign.setTrue(0);
				Assert::IsTrue(clause.evaluate(assign));

				assign.setTrue(1);
				Assert::IsTrue(clause.evaluate(assign));

				assign.setFalse(0);
				Assert::IsTrue(clause.evaluate(assign));
			}

			TEST_METHOD(Clause_OrLiteralsEvaluateNegated) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				Clause clause{ move(literal1), move(literal2), Operator_e::Or };
				clause.negate();

				Assignment assign{ 2 };
				Assert::IsTrue(clause.evaluate(assign));

				assign.setTrue(0);
				Assert::IsTrue(!clause.evaluate(assign));

				assign.setTrue(1);
				Assert::IsTrue(!clause.evaluate(assign));

				assign.setFalse(0);
				Assert::IsTrue(!clause.evaluate(assign));
			}

			TEST_METHOD(Clause_OrClausesConstruct) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1, true);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> literal4 = make_unique<Literal>(3, str4);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::And);
				unique_ptr<Expression> clause2 = make_unique<Clause>(move(literal3), move(literal4), Operator_e::Implies);
				Clause clause3{ move(clause1), move(clause2), Operator_e::Or };

				stringstream sstream{};
				sstream << clause3;

				stringstream sresult{};
				sresult << '('
					<< '(' << kNotSymbol << str1 << ' ' << kAndSymbol << ' ' << str2 << ')'
					<< ' ' << kOrSymbol << ' '
					<< '(' << str3 << ' ' << kImpliesSymbol << ' ' << str4 << ')'
					<< ')';

				Assert::IsTrue(sstream.str() == sresult.str());
			}

			TEST_METHOD(Clause_OrClausesNegate) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1, true);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> literal4 = make_unique<Literal>(3, str4);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::And);
				unique_ptr<Expression> clause2 = make_unique<Clause>(move(literal3), move(literal4), Operator_e::Implies);
				Clause clause3{ move(clause1), move(clause2), Operator_e::Or };
				clause3.negate();

				stringstream sstream{};
				sstream << clause3;

				stringstream sresult{};
				sresult << '('
					<< '(' << str1 << ' ' << kOrSymbol << ' ' << kNotSymbol << str2 << ')'
					<< ' ' << kAndSymbol << ' '
					<< '(' << str3 << ' ' << kAndSymbol << ' ' << kNotSymbol << str4 << ')'
					<< ')';

				Assert::IsTrue(sstream.str() == sresult.str());
			}

			TEST_METHOD(Clause_OrClausesEvaluate) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1, true);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> literal4 = make_unique<Literal>(3, str4);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::And);
				unique_ptr<Expression> clause2 = make_unique<Clause>(move(literal3), move(literal4), Operator_e::Implies);
				Clause clause3{ move(clause1), move(clause2), Operator_e::Or };

				Assignment assign{ 4 };
				assign.setTrue(0);
				assign.setTrue(2);
				Assert::IsTrue(!clause3.evaluate(assign));

				assign.setFalse(0);
				assign.setTrue(1);
				Assert::IsTrue(clause3.evaluate(assign));

				assign.setTrue(3);
				Assert::IsTrue(clause3.evaluate(assign));

				assign.setTrue(0);
				Assert::IsTrue(clause3.evaluate(assign));
			}

			TEST_METHOD(Clause_OrClausesEvaluateNegated) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1, true);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> literal4 = make_unique<Literal>(3, str4);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::And);
				unique_ptr<Expression> clause2 = make_unique<Clause>(move(literal3), move(literal4), Operator_e::Implies);
				Clause clause3{ move(clause1), move(clause2), Operator_e::Or };
				clause3.negate();

				Assignment assign{ 4 };
				assign.setTrue(0);
				assign.setTrue(2);
				Assert::IsTrue(clause3.evaluate(assign));

				assign.setFalse(0);
				assign.setTrue(1);
				Assert::IsTrue(!clause3.evaluate(assign));

				assign.setTrue(3);
				Assert::IsTrue(!clause3.evaluate(assign));

				assign.setTrue(0);
				Assert::IsTrue(!clause3.evaluate(assign));
			}

			TEST_METHOD(Clause_OrMixedConstruct) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::Implies);
				Clause clause2{ move(literal3), move(clause1), Operator_e::Or };

				stringstream sstream{};
				sstream << clause2;

				stringstream sresult{};
				sresult << '('
					<< str3
					<< ' ' << kOrSymbol << ' '
					<< '(' << str1 << ' ' << kImpliesSymbol << ' ' << str2 << ')'
					<< ')';

				Assert::IsTrue(sstream.str() == sresult.str());
			}

			TEST_METHOD(Clause_OrMixedNegate) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::Implies);
				Clause clause2{ move(literal3), move(clause1), Operator_e::Or };
				clause2.negate();

				stringstream sstream{};
				sstream << clause2;

				stringstream sresult{};
				sresult << '('
					<< kNotSymbol << str3
					<< ' ' << kAndSymbol << ' '
					<< '(' << str1 << ' ' << kAndSymbol << ' ' << kNotSymbol << str2 << ')'
					<< ')';

				Assert::IsTrue(sstream.str() == sresult.str());
			}

			TEST_METHOD(Clause_OrMixedEvaluate) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::Implies);
				Clause clause2{ move(literal3), move(clause1), Operator_e::Or };

				Assignment assign{ 4 };
				assign.setTrue(0);
				Assert::IsTrue(!clause2.evaluate(assign));

				assign.setTrue(2);
				Assert::IsTrue(clause2.evaluate(assign));

				assign.setTrue(1);
				Assert::IsTrue(clause2.evaluate(assign));

				assign.setFalse(2);
				Assert::IsTrue(clause2.evaluate(assign));
			}

			TEST_METHOD(Clause_OrMixedEvaluateNegated) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::Implies);
				Clause clause2{ move(literal3), move(clause1), Operator_e::Or };
				clause2.negate();

				Assignment assign{ 3 };
				assign.setTrue(0);
				Assert::IsTrue(clause2.evaluate(assign));

				assign.setTrue(2);
				Assert::IsTrue(!clause2.evaluate(assign));

				assign.setTrue(1);
				Assert::IsTrue(!clause2.evaluate(assign));

				assign.setFalse(2);
				Assert::IsTrue(!clause2.evaluate(assign));
			}

			TEST_METHOD(Clause_ImpliesLiteralsConstruct) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				Clause clause{ move(literal1), move(literal2), Operator_e::Implies };

				stringstream sstream{};
				sstream << clause;

				stringstream sresult{};
				sresult << '(' << str1 << ' ' << kImpliesSymbol << ' ' << str2 << ')';

				Assert::IsTrue(sstream.str() == sresult.str());
			}

			TEST_METHOD(Clause_ImpliesLiteralsNegate) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				Clause clause{ move(literal1), move(literal2), Operator_e::Implies };
				clause.negate();

				stringstream sstream{};
				sstream << clause;

				stringstream sresult{};
				sresult << '(' << str1 << ' ' << kAndSymbol << ' ' << kNotSymbol << str2 << ')';

				Assert::IsTrue(sstream.str() == sresult.str());
			}

			TEST_METHOD(Clause_ImpliesLiteralsEvaluate) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				Clause clause{ move(literal1), move(literal2), Operator_e::Implies };

				Assignment assign{ 2 };
				Assert::IsTrue(clause.evaluate(assign));

				assign.setTrue(0);
				Assert::IsTrue(!clause.evaluate(assign));

				assign.setTrue(1);
				Assert::IsTrue(clause.evaluate(assign));

				assign.setFalse(0);
				Assert::IsTrue(clause.evaluate(assign));
			}

			TEST_METHOD(Clause_ImpliesLiteralsEvaluateNegated) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				Clause clause{ move(literal1), move(literal2), Operator_e::Implies };
				clause.negate();

				Assignment assign{ 2 };
				Assert::IsTrue(!clause.evaluate(assign));

				assign.setTrue(0);
				Assert::IsTrue(clause.evaluate(assign));

				assign.setTrue(1);
				Assert::IsTrue(!clause.evaluate(assign));

				assign.setFalse(0);
				Assert::IsTrue(!clause.evaluate(assign));
		}

			TEST_METHOD(Clause_ImpliesClausesConstruct) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1, true);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> literal4 = make_unique<Literal>(3, str4);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::And);
				unique_ptr<Expression> clause2 = make_unique<Clause>(move(literal3), move(literal4), Operator_e::Or);
				Clause clause3{ move(clause1), move(clause2), Operator_e::Implies };

				stringstream sstream{};
				sstream << clause3;

				stringstream sresult{};
				sresult << '('
					<< '(' << kNotSymbol << str1 << ' ' << kAndSymbol << ' ' << str2 << ')'
					<< ' ' << kImpliesSymbol << ' '
					<< '(' << str3 << ' ' << kOrSymbol << ' ' << str4 << ')'
					<< ')';

				Assert::IsTrue(sstream.str() == sresult.str());
			}

			TEST_METHOD(Clause_ImpliesClausesNegate) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1, true);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> literal4 = make_unique<Literal>(3, str4);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::And);
				unique_ptr<Expression> clause2 = make_unique<Clause>(move(literal3), move(literal4), Operator_e::Or);
				Clause clause3{ move(clause1), move(clause2), Operator_e::Implies };
				clause3.negate();

				stringstream sstream{};
				sstream << clause3;

				stringstream sresult{};
				sresult << '('
					<< '(' << kNotSymbol << str1 << ' ' << kAndSymbol << ' ' << str2 << ')'
					<< ' ' << kAndSymbol << ' '
					<< '(' << kNotSymbol << str3 << ' ' << kAndSymbol << ' ' << kNotSymbol << str4 << ')'
					<< ')';

				Assert::IsTrue(sstream.str() == sresult.str());
			}

			TEST_METHOD(Clause_ImpliesClausesEvaluate) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1, true);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> literal4 = make_unique<Literal>(3, str4);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::And);
				unique_ptr<Expression> clause2 = make_unique<Clause>(move(literal3), move(literal4), Operator_e::Or);
				Clause clause3{ move(clause1), move(clause2), Operator_e::Implies };

				Assignment assign{ 4 };
				assign.setTrue(0);
				assign.setTrue(1);
				Assert::IsTrue(clause3.evaluate(assign));

				assign.setFalse(0);
				Assert::IsTrue(!clause3.evaluate(assign));

				assign.setTrue(2);
				assign.setTrue(3);
				Assert::IsTrue(clause3.evaluate(assign));

				assign.setTrue(0);
				assign.setFalse(1);
				Assert::IsTrue(clause3.evaluate(assign));
			}

			TEST_METHOD(Clause_ImpliesClausesEvaluateNegated) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1, true);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> literal4 = make_unique<Literal>(3, str4);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::And);
				unique_ptr<Expression> clause2 = make_unique<Clause>(move(literal3), move(literal4), Operator_e::Or);
				Clause clause3{ move(clause1), move(clause2), Operator_e::Implies };
				clause3.negate();

				Assignment assign{ 4 };
				assign.setTrue(0);
				assign.setTrue(1);
				Assert::IsTrue(!clause3.evaluate(assign));

				assign.setFalse(0);
				Assert::IsTrue(clause3.evaluate(assign));

				assign.setTrue(2);
				assign.setTrue(3);
				Assert::IsTrue(!clause3.evaluate(assign));

				assign.setTrue(0);
				assign.setFalse(1);
				Assert::IsTrue(!clause3.evaluate(assign));
			}

			TEST_METHOD(Clause_ImpliesMixedConstruct) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::Xor);
				Clause clause2{ move(clause1), move(literal3), Operator_e::Implies };

				stringstream sstream{};
				sstream << clause2;

				stringstream sresult{};
				sresult << '('
					<< '(' << str1 << ' ' << kXorSymbol << ' ' << str2 << ')'
					<< ' ' << kImpliesSymbol << ' '
					<< str3
					<< ')';

				Assert::IsTrue(sstream.str() == sresult.str());
			}

			TEST_METHOD(Clause_ImpliesMixedNegate) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::Xor);
				Clause clause2{ move(clause1), move(literal3), Operator_e::Implies };
				clause2.negate();

				stringstream sstream{};
				sstream << clause2;

				stringstream sresult{};
				sresult << '('
					<< '(' << str1 << ' ' << kXorSymbol << ' ' << str2 << ')'
					<< ' ' << kAndSymbol << ' '
					<< kNotSymbol << str3
					<< ')';

				Assert::IsTrue(sstream.str() == sresult.str());
			}

			TEST_METHOD(Clause_ImpliesMixedEvaluate) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::Xor);
				Clause clause2{ move(clause1), move(literal3), Operator_e::Implies };

				Assignment assign{ 3 };
				Assert::IsTrue(clause2.evaluate(assign));

				assign.setTrue(0);
				Assert::IsTrue(!clause2.evaluate(assign));

				assign.setTrue(2);
				Assert::IsTrue(clause2.evaluate(assign));

				assign.setFalse(0);
				Assert::IsTrue(clause2.evaluate(assign));
			}

			TEST_METHOD(Clause_ImpliesMixedEvaluateNegated) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::Xor);
				Clause clause2{ move(clause1), move(literal3), Operator_e::Implies };
				clause2.negate();

				Assignment assign{ 3 };
				Assert::IsTrue(!clause2.evaluate(assign));

				assign.setTrue(0);
				Assert::IsTrue(clause2.evaluate(assign));

				assign.setTrue(2);
				Assert::IsTrue(!clause2.evaluate(assign));

				assign.setFalse(0);
				Assert::IsTrue(!clause2.evaluate(assign));
			}

			TEST_METHOD(Clause_XorLiteralsConstruct) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				Clause clause{ move(literal1), move(literal2), Operator_e::Xor };

				stringstream sstream{};
				sstream << clause;

				stringstream sresult{};
				sresult << '(' << str1 << ' ' << kXorSymbol << ' ' << str2 << ')';

				Assert::IsTrue(sstream.str() == sresult.str());
			}

			TEST_METHOD(Clause_XorLiteralsNegate) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				Clause clause{ move(literal1), move(literal2), Operator_e::Xor };
				clause.negate();

				stringstream sstream{};
				sstream << clause;

				stringstream sresult{};
				sresult << '('
					<< '(' << str1 << ' ' << kAndSymbol << ' ' << str2 << ')'
					<< ' ' << kOrSymbol << ' '
					<< '(' << kNotSymbol << str1 << ' ' << kAndSymbol << ' ' << kNotSymbol << str2 << ')'
					<< ')';

				Assert::IsTrue(sstream.str() == sresult.str());
			}

			TEST_METHOD(Clause_XorLiteralsEvaluate) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				Clause clause{ move(literal1), move(literal2), Operator_e::Xor };

				Assignment assign{ 2 };
				Assert::IsTrue(!clause.evaluate(assign));

				assign.setTrue(0);
				Assert::IsTrue(clause.evaluate(assign));

				assign.setTrue(1);
				Assert::IsTrue(!clause.evaluate(assign));

				assign.setFalse(0);
				Assert::IsTrue(clause.evaluate(assign));
			}

			TEST_METHOD(Clause_XorLiteralsEvaluateNegated) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				Clause clause{ move(literal1), move(literal2), Operator_e::Xor };
				clause.negate();

				Assignment assign{ 2 };
				Assert::IsTrue(clause.evaluate(assign));

				assign.setTrue(0);
				Assert::IsTrue(!clause.evaluate(assign));

				assign.setTrue(1);
				Assert::IsTrue(clause.evaluate(assign));

				assign.setFalse(0);
				Assert::IsTrue(!clause.evaluate(assign));
			}

			TEST_METHOD(Clause_XorClausesConstruct) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1, true);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> literal4 = make_unique<Literal>(3, str4);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::Or);
				unique_ptr<Expression> clause2 = make_unique<Clause>(move(literal3), move(literal4), Operator_e::Or);
				Clause clause3{ move(clause1), move(clause2), Operator_e::Xor };

				stringstream sstream{};
				sstream << clause3;

				stringstream sresult{};
				sresult << '('
					<< '(' << kNotSymbol << str1 << ' ' << kOrSymbol << ' ' << str2 << ')'
					<< ' ' << kXorSymbol << ' '
					<< '(' << str3 << ' ' << kOrSymbol << ' ' << str4 << ')'
					<< ')';

				Assert::IsTrue(sstream.str() == sresult.str());
			}

			TEST_METHOD(Clause_XorClausesNegate) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1, true);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> literal4 = make_unique<Literal>(3, str4);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::Or);
				unique_ptr<Expression> clause2 = make_unique<Clause>(move(literal3), move(literal4), Operator_e::Or);
				Clause clause3{ move(clause1), move(clause2), Operator_e::Xor };
				clause3.negate();

				stringstream sstream{};
				sstream << clause3;

				stringstream sresult{};
				sresult << '('
					<< '('
					<< '(' << kNotSymbol << str1 << ' ' << kOrSymbol << ' ' << str2 << ')'
					<< ' ' << kAndSymbol << ' '
					<< '(' << str3 << ' ' << kOrSymbol << ' ' << str4 << ')'
					<< ')'
					<< ' ' << kOrSymbol << ' '
					<< '('
					<< '(' << str1 << ' ' << kAndSymbol << ' ' << kNotSymbol << str2 << ')'
					<< ' ' << kAndSymbol << ' '
					<< '(' << kNotSymbol << str3 << ' ' << kAndSymbol << ' ' << kNotSymbol << str4 << ')'
					<< ')'
					<< ')';

				Assert::IsTrue(sstream.str() == sresult.str());
			}

			TEST_METHOD(Clause_XorClausesEvaluate) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1, true);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> literal4 = make_unique<Literal>(3, str4);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::Or);
				unique_ptr<Expression> clause2 = make_unique<Clause>(move(literal3), move(literal4), Operator_e::Or);
				Clause clause3{ move(clause1), move(clause2), Operator_e::Xor };

				Assignment assign{ 4 };
				assign.setTrue(0);
				Assert::IsTrue(!clause3.evaluate(assign));

				assign.setFalse(0);
				Assert::IsTrue(clause3.evaluate(assign));

				assign.setTrue(3);
				Assert::IsTrue(!clause3.evaluate(assign));

				assign.setTrue(0);
				Assert::IsTrue(clause3.evaluate(assign));
			}

			TEST_METHOD(Clause_XorClausesEvaluateNegated) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1, true);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> literal4 = make_unique<Literal>(3, str4);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::Or);
				unique_ptr<Expression> clause2 = make_unique<Clause>(move(literal3), move(literal4), Operator_e::Or);
				Clause clause3{ move(clause1), move(clause2), Operator_e::Xor };
				clause3.negate();

				Assignment assign{ 4 };
				assign.setTrue(0);
				Assert::IsTrue(clause3.evaluate(assign));

				assign.setFalse(0);
				Assert::IsTrue(!clause3.evaluate(assign));

				assign.setTrue(3);
				Assert::IsTrue(clause3.evaluate(assign));

				assign.setTrue(0);
				Assert::IsTrue(!clause3.evaluate(assign));
			}

			TEST_METHOD(Clause_XorMixedConstruct) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::And);
				Clause clause2{ move(literal3), move(clause1), Operator_e::Xor };

				stringstream sstream{};
				sstream << clause2;

				stringstream sresult{};
				sresult << '('
					<< str3
					<< ' ' << kXorSymbol << ' '
					<< '(' << str1 << ' ' << kAndSymbol << ' ' << str2 << ')'
					<< ')';

				Assert::IsTrue(sstream.str() == sresult.str());
			}

			TEST_METHOD(Clause_XorMixedNegate) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::And);
				Clause clause2{ move(literal3), move(clause1), Operator_e::Xor };
				clause2.negate();

				stringstream sstream{};
				sstream << clause2;

				stringstream sresult{};
				sresult << '('
					<< '('
					<< str3
					<< ' ' << kAndSymbol << ' '
					<< '(' << str1 << ' ' << kAndSymbol << ' ' << str2 << ')'
					<< ')'
					<< ' ' << kOrSymbol << ' '
					<< '('
					<< kNotSymbol << str3
					<< ' ' << kAndSymbol << ' '
					<< '(' << kNotSymbol << str1 << ' ' << kOrSymbol << ' ' << kNotSymbol << str2 << ')'
					<< ')'
					<< ')';

				Assert::IsTrue(sstream.str() == sresult.str());
			}

			TEST_METHOD(Clause_XorMixedEvaluate) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::And);
				Clause clause2{ move(literal3), move(clause1), Operator_e::Xor };

				Assignment assign{ 3 };
				Assert::IsTrue(!clause2.evaluate(assign));

				assign.setTrue(2);
				Assert::IsTrue(clause2.evaluate(assign));

				assign.setTrue(0);
				assign.setTrue(1);
				Assert::IsTrue(!clause2.evaluate(assign));

				assign.setFalse(2);
				Assert::IsTrue(clause2.evaluate(assign));
			}

			TEST_METHOD(Clause_XorMixedEvaluateNegated) {
				unique_ptr<Expression> literal1 = make_unique<Literal>(0, str1);
				unique_ptr<Expression> literal2 = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literal3 = make_unique<Literal>(2, str3);
				unique_ptr<Expression> clause1 = make_unique<Clause>(move(literal1), move(literal2), Operator_e::And);
				Clause clause2{ move(literal3), move(clause1), Operator_e::Xor };
				clause2.negate();

				Assignment assign{ 3 };
				Assert::IsTrue(clause2.evaluate(assign));

				assign.setTrue(2);
				Assert::IsTrue(!clause2.evaluate(assign));

				assign.setTrue(0);
				assign.setTrue(1);
				Assert::IsTrue(clause2.evaluate(assign));

				assign.setFalse(2);
				Assert::IsTrue(!clause2.evaluate(assign));
			}

			TEST_METHOD(Clause_DeepNestPrint) {
				unique_ptr<Expression> literalA = make_unique<Literal>(0, str1, true);
				unique_ptr<Expression> literalB = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literalC = make_unique<Literal>(2, str3);
				unique_ptr<Expression> literalD = make_unique<Literal>(3, str4);
				unique_ptr<Expression> literalE = make_unique<Literal>(4, str5);
				unique_ptr<Expression> literalF = make_unique<Literal>(5, str6, true);

				unique_ptr<Expression> clauseBorC = make_unique<Clause>(move(literalB), move(literalC), Operator_e::Or);
				unique_ptr<Expression> clauseDimpE = make_unique<Clause>(move(literalD), move(literalE), Operator_e::Implies);
				unique_ptr<Expression> clauseXor = make_unique<Clause>(move(clauseBorC), move(clauseDimpE), Operator_e::Xor);
				unique_ptr<Expression> clauseNotF = make_unique<Clause>(move(clauseXor), move(literalF), Operator_e::Or);
				Clause clauseWhole{ move(literalA), move(clauseNotF), Operator_e::And };

				stringstream sstream{};
				sstream << clauseWhole;

				stringstream sresult{};
				sresult << '('
					<< kNotSymbol << str1
					<< ' ' << kAndSymbol << ' '
					<< '('
					<< '('
					<< '(' << str2 << ' ' << kOrSymbol << ' ' << str3 << ')'
					<< ' ' << kXorSymbol << ' '
					<< '(' << str4 << ' ' << kImpliesSymbol << ' ' << str5 << ')'
					<< ')'
					<< ' ' << kOrSymbol << ' '
					<< kNotSymbol << str6
					<< ')'
					<< ')';

				Assert::IsTrue(sstream.str() == sresult.str());
			}

			TEST_METHOD(Clause_DeepNestNegate) {
				unique_ptr<Expression> literalA = make_unique<Literal>(0, str1, true);
				unique_ptr<Expression> literalB = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literalC = make_unique<Literal>(2, str3);
				unique_ptr<Expression> literalD = make_unique<Literal>(3, str4);
				unique_ptr<Expression> literalE = make_unique<Literal>(4, str5);
				unique_ptr<Expression> literalF = make_unique<Literal>(5, str6, true);

				unique_ptr<Expression> clauseBorC = make_unique<Clause>(move(literalB), move(literalC), Operator_e::Or);
				unique_ptr<Expression> clauseDimpE = make_unique<Clause>(move(literalD), move(literalE), Operator_e::Implies);
				unique_ptr<Expression> clauseXor = make_unique<Clause>(move(clauseBorC), move(clauseDimpE), Operator_e::Xor);
				unique_ptr<Expression> clauseNotF = make_unique<Clause>(move(clauseXor), move(literalF), Operator_e::Or);
				Clause clauseWhole{ move(literalA), move(clauseNotF), Operator_e::And };
				clauseWhole.negate();

				stringstream sstream{};
				sstream << clauseWhole;

				stringstream sresult{};
				sresult << '('
					<< str1
					<< ' ' << kOrSymbol << ' '
					<< '('
					<< '('
					<< '('
					<< '(' << str2 << ' ' << kOrSymbol << ' ' << str3 << ')'
					<< ' ' << kAndSymbol << ' '
					<< '(' << str4 << ' ' << kImpliesSymbol << ' ' << str5 << ')'
					<< ')'
					<< ' ' << kOrSymbol << ' '
					<< '('
					<< '(' << kNotSymbol << str2 << ' ' << kAndSymbol << ' ' << kNotSymbol << str3 << ')'
					<< ' ' << kAndSymbol << ' '
					<< '(' << str4 << ' ' << kAndSymbol << ' ' << kNotSymbol << str5 << ')'
					<< ')'
					<< ')'
					<< ' ' << kAndSymbol << ' ' << str6
					<< ')'
					<< ')';

				Assert::IsTrue(sstream.str() == sresult.str());
			}

			TEST_METHOD(Clause_DeepNestEvaluate) {
				unique_ptr<Expression> literalA = make_unique<Literal>(0, str1, true);
				unique_ptr<Expression> literalB = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literalC = make_unique<Literal>(2, str3);
				unique_ptr<Expression> literalD = make_unique<Literal>(3, str4);
				unique_ptr<Expression> literalE = make_unique<Literal>(4, str5);
				unique_ptr<Expression> literalF = make_unique<Literal>(5, str6, true);

				unique_ptr<Expression> clauseBorC = make_unique<Clause>(move(literalB), move(literalC), Operator_e::Or);
				unique_ptr<Expression> clauseDimpE = make_unique<Clause>(move(literalD), move(literalE), Operator_e::Implies);
				unique_ptr<Expression> clauseXor = make_unique<Clause>(move(clauseBorC), move(clauseDimpE), Operator_e::Xor);
				unique_ptr<Expression> clauseNotF = make_unique<Clause>(move(clauseXor), move(literalF), Operator_e::Or);
				Clause clauseWhole{ move(literalA), move(clauseNotF), Operator_e::And };

				Assignment assign{ 6 };
				assign.setTrue(2);
				assign.setTrue(3);
				assign.setTrue(5);
				Assert::IsTrue(clauseWhole.evaluate(assign));

				assign.setFalse(3);
				Assert::IsTrue(!clauseWhole.evaluate(assign));
			}

			TEST_METHOD(Clause_DeepNestEvaluateNegated) {
				unique_ptr<Expression> literalA = make_unique<Literal>(0, str1, true);
				unique_ptr<Expression> literalB = make_unique<Literal>(1, str2);
				unique_ptr<Expression> literalC = make_unique<Literal>(2, str3);
				unique_ptr<Expression> literalD = make_unique<Literal>(3, str4);
				unique_ptr<Expression> literalE = make_unique<Literal>(4, str5);
				unique_ptr<Expression> literalF = make_unique<Literal>(5, str6, true);

				unique_ptr<Expression> clauseBorC = make_unique<Clause>(move(literalB), move(literalC), Operator_e::Or);
				unique_ptr<Expression> clauseDimpE = make_unique<Clause>(move(literalD), move(literalE), Operator_e::Implies);
				unique_ptr<Expression> clauseXor = make_unique<Clause>(move(clauseBorC), move(clauseDimpE), Operator_e::Xor);
				unique_ptr<Expression> clauseNotF = make_unique<Clause>(move(clauseXor), move(literalF), Operator_e::Or);
				Clause clauseWhole{ move(literalA), move(clauseNotF), Operator_e::And };
				clauseWhole.negate();

				Assignment assign{ 6 };
				assign.setTrue(2);
				assign.setTrue(3);
				assign.setTrue(5);
				Assert::IsTrue(!clauseWhole.evaluate(assign));

				assign.setFalse(3);
				Assert::IsTrue(clauseWhole.evaluate(assign));
			}

		private:
			string str1{ "A" };
			string str2{ "B" };
			string str3{ "C" };
			string str4{ "D" };
			string str5{ "E" };
			string str6{ "F" };
	};
}