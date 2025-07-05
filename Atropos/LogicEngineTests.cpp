#include "stdafx.h"												// for UnitTesting framework
#include "CppUnitTest.h"										// for UnitTesting framework
#include "../Eunomia/Category.h"								// for Category
#include "../Eunomia/Clause.h"									// for Clause
#include "../Eunomia/Expression.h"								// for Expression
#include "../Eunomia/Literal.h"									// for Literal
#include "../Eunomia/LogicEngine.h"								// for LogicEngine
#include "../Eunomia/Puzzle.h"									// for Puzzle
#include "../Eunomia/Utility.h"									// for VarStatus_e
#include <memory>												// for unique_ptr, make_unique
#include <string>												// for string
#include <vector>												// for vector

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using std::string; using std::to_string;
using std::vector;
using std::unique_ptr; using std::make_unique;


static unique_ptr<Expression> makeExpr(const Literal& literal, bool negate = false) {
	auto expr = make_unique<Literal>(literal);
	if (negate) {
		expr->negate();
	}
	return expr;
}

namespace Atropos {
	TEST_CLASS(LogicEngineTester) {
		public:
			TEST_METHOD(LogicEngine_Construct) {
				Puzzle puzzle = buildPuzzle();
				LogicEngine engine{ puzzle.numCategories(), puzzle.itemsPerCategory() };

				size_t num = puzzle.numVariables();
				for (size_t i = 0; i < num; ++i) {
					Assert::IsTrue(engine.getStatus(i) == VarStatus_e::Unknown);
				}
				Assert::IsTrue(!engine.solved());
			}

			TEST_METHOD(LogicEngine_SolveVariableTrue) {
				Puzzle puzzle = buildPuzzle();
				LogicEngine engine{ puzzle.numCategories(), puzzle.itemsPerCategory() };
				auto literals = getLiterals();

				engine.evaluateClue(literals[14]);
				engine.evaluateClue(literals[23]);

				Assert::IsTrue(engine.getStatus(0) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(1) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(2) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(3) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(4) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(5) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(6) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(7) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(8) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(9) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(10) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(11) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(12) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(13) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(14) == VarStatus_e::True);
				Assert::IsTrue(engine.getStatus(15) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(16) == VarStatus_e::True);
				Assert::IsTrue(engine.getStatus(17) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(18) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(19) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(20) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(21) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(22) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(23) == VarStatus_e::True);
				Assert::IsTrue(engine.getStatus(24) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(25) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(26) == VarStatus_e::False);
				Assert::IsTrue(!engine.solved());
			}

			TEST_METHOD(LogicEngine_SolveVariableFalse) {
				Puzzle puzzle = buildPuzzle();
				LogicEngine engine{ puzzle.numCategories(), puzzle.itemsPerCategory() };
				auto literals = getLiterals();

				Literal lit{ literals[9] };
				lit.negate();
				engine.evaluateClue(lit);

				Assert::IsTrue(engine.getStatus(0) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(1) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(2) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(3) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(4) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(5) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(6) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(7) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(8) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(9) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(10) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(11) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(12) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(13) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(14) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(15) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(16) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(17) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(18) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(19) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(20) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(21) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(22) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(23) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(24) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(25) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(26) == VarStatus_e::Unknown);
				Assert::IsTrue(!engine.solved());
			}

			TEST_METHOD(LogicEngine_ApplyClues) {
				Puzzle puzzle = buildPuzzle();
				LogicEngine engine{ puzzle.numCategories(), puzzle.itemsPerCategory() };
				auto literals = getLiterals();

				auto c1 = make_unique<Clause>(makeExpr(literals[6]), makeExpr(literals[8]), Operator_e::Or);
				auto c2 = make_unique<Clause>(
					make_unique<Clause>(makeExpr(literals[2], true), makeExpr(literals[22], true), Operator_e::And),
					makeExpr(literals[4]), Operator_e::And);
				engine.evaluateClue(*c1);
				engine.evaluateClue(*c2);

				Assert::IsTrue(engine.getStatus(0) == VarStatus_e::True);
				Assert::IsTrue(engine.getStatus(1) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(2) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(3) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(4) == VarStatus_e::True);
				Assert::IsTrue(engine.getStatus(5) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(6) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(7) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(8) == VarStatus_e::True);
				Assert::IsTrue(engine.getStatus(9) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(10) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(11) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(12) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(13) == VarStatus_e::True);
				Assert::IsTrue(engine.getStatus(14) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(15) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(16) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(17) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(18) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(19) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(20) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(21) == VarStatus_e::True);
				Assert::IsTrue(engine.getStatus(22) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(23) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(24) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(25) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(26) == VarStatus_e::Unknown);
				Assert::IsTrue(!engine.solved());
			}

			TEST_METHOD(LogicEngine_Solve) {
				Puzzle puzzle = buildPuzzle();
				LogicEngine engine{ puzzle.numCategories(), puzzle.itemsPerCategory() };
				auto literals = getLiterals();

				auto c1 = make_unique<Clause>(makeExpr(literals[6]), makeExpr(literals[8]), Operator_e::Or);
				auto c2 = make_unique<Clause>(
					make_unique<Clause>(makeExpr(literals[2], true), makeExpr(literals[22], true), Operator_e::And),
					makeExpr(literals[4]), Operator_e::And);
				auto c3 = make_unique<Clause>(makeExpr(literals[17]), makeExpr(literals[14]), Operator_e::Implies);
				engine.evaluateClue(*c1);
				engine.evaluateClue(*c2);
				engine.evaluateClue(*c3);

				Assert::IsTrue(engine.getStatus(0) == VarStatus_e::True);
				Assert::IsTrue(engine.getStatus(1) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(2) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(3) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(4) == VarStatus_e::True);
				Assert::IsTrue(engine.getStatus(5) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(6) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(7) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(8) == VarStatus_e::True);
				Assert::IsTrue(engine.getStatus(9) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(10) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(11) == VarStatus_e::True);
				Assert::IsTrue(engine.getStatus(12) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(13) == VarStatus_e::True);
				Assert::IsTrue(engine.getStatus(14) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(15) == VarStatus_e::True);
				Assert::IsTrue(engine.getStatus(16) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(17) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(18) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(19) == VarStatus_e::True);
				Assert::IsTrue(engine.getStatus(20) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(21) == VarStatus_e::True);
				Assert::IsTrue(engine.getStatus(22) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(23) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(24) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(25) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(26) == VarStatus_e::True);
				Assert::IsTrue(engine.solved());
			}

			TEST_METHOD(LogicEngine_Inconsistent) {
				Puzzle puzzle = buildPuzzle();
				LogicEngine engine{ puzzle.numCategories(), puzzle.itemsPerCategory() };
				auto literals = getLiterals();

				auto c1 = makeExpr(literals[7]);
				auto c2 = makeExpr(literals[7], true);
				auto c3 = makeExpr(literals[26]);

				Assert::IsTrue(engine.evaluateClue(*c1));
				Assert::IsTrue(!engine.evaluateClue(*c2));
				Assert::IsTrue(engine.evaluateClue(*c3));

				Assert::IsTrue(engine.getStatus(0) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(1) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(2) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(3) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(4) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(5) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(6) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(7) == VarStatus_e::True);
				Assert::IsTrue(engine.getStatus(8) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(9) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(10) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(11) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(12) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(13) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(14) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(15) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(16) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(17) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(18) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(19) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(20) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(21) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(22) == VarStatus_e::Unknown);
				Assert::IsTrue(engine.getStatus(23) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(24) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(25) == VarStatus_e::False);
				Assert::IsTrue(engine.getStatus(26) == VarStatus_e::True);
				Assert::IsTrue(!engine.solved());
			}

		private:
			Puzzle buildPuzzle() const {
				vector<string> items1{ "Leopard", "Ocelot", "Tiger" };
				vector<string> items2{ "Quetzalcoatl", "Huitzilopochtli", "Tlaloc" };
				vector<string> items3{ "Pumpernickel", "Whole Wheat", "Focaccia" };

				vector<Category> categories{
					Category{ "Felines", items1.cbegin(), items1.cend() },
					Category{ "Aztec Gods", items2.cbegin(), items2.cend() },
					Category{ "Breads", items3.cbegin(), items3.cend() }
				};
				return Puzzle{ "LogicEngine Testing Puzzle", "A puzzle used to test the LogicEngine",
					categories.cbegin(), categories.cend() };
			}

			vector<Literal> getLiterals() const {
				Puzzle p = buildPuzzle();
				size_t num = p.numVariables();
				
				vector<Literal> literals;
				literals.reserve(num);
				for (size_t i = 0; i < num; ++i) {
					literals.emplace_back(i, to_string(i));
				}
				return literals;
			}
	};
}