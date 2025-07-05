#ifndef EUNOMIA_LITERAL
#define EUNOMIA_LITERAL

#include <iosfwd>												// for ostream
#include <string>												// for string
#include "Expression.h"											// for Expression

class Assignment;

/*
	A Literal is a kind of Expression that consists of a single variable that may or may not be negated.
	Each Literal corresponds to a specific variable in a set; this variable is identified by an ID number
	with which the Literal is constructed. A Literal also has a "name," which is the string representation
	that it uses for the purposes of printing.

	If a Literal is negated, applying a negation results in the same Literal but not negated. The negation
	of a Literal that is not negated is the same Literal but negated. The Evaluation of a Literal depends
	solely on the assignment of the underlying variable; however, if the Literal is negated, the evaluation
	is the Boolean opposite of that assignment.

	A Literal prints without any parentheses or other grouping markers. The "name" of the Literal is
	preceded by a negation character if the Literal is negated.
*/

class Literal : public Expression {
	public:
		// [Constructor]
		// REQUIRES: <id> is non-negative, <name> is non-empty
		Literal(int id, const std::string& name, bool isNeg = false);

		// [Negator]
		// MODIFIES: <this>
		// EFFECTS:  negates <this>
		void negate() override;

		// [Evaluator]
		// REQUIRES: each variable that forms part of <this> is within the scope of <assign>
		// EFFECTS:  uses the variable assignments described by <assign> to evaluate <this>
		//   and returns the evaluation, either TRUE or FALSE
		bool evaluate(const Assignment& assign) const override;

	protected:
		// [Printer]
		// MODIFIES: <os>
		// EFFECTS:  prints <this> to <os> without a trailing newline character
		void print(std::ostream& os) const override;

	private:
		std::string name;
		int varID;
		bool negated;
};

#endif