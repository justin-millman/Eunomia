#ifndef EUNOMIA_CLAUSE
#define EUNOMIA_CLAUSE

#include <iosfwd>												// for ostream
#include <memory>												// for unique_ptr
#include "Expression.h"											// for Expression
#include "Utility.h"											// for Operator_e

class Assignment;

/*
	A Clause is a kind of Expression that consists of two Expressions joined together by a Boolean operator.
	The two Expressions need not be the same kind of Expression, but they don't need to be different either.
	A Clause may be negated at construction.

	If a Clause is negated, applying an egation results in the same Clause but not negated. The negation
	of a Clause this is not negated is the same Clause but negated. The negation will affect the print output
	of a Clause, as the negation is propogated through the two constituent Expressions, which themselves
	may print differently when negated.

	A Clause is evaluated by independently evaluating the two constituent expressions and applying the
	Boolean operator to those individual evaluations. If a Clause is negated, the result of applying the
	Boolean operator to the two individaul evaluations is itself negated to obtain the final result.

	When a Clause is printed, it is wholly enclosed in a pair of parentheses. Inside the parentheses, the
	two constituent Expressions are independently printed, and a character representing the Boolean
	operator is printed between them. A Clause is never preceded by a negation character; rather, the
	negation of a negated Clause is propogated to the constituent Expressions for the purposes of printing.
*/

class Clause : public Expression {
	public:
		// [Constructor]
		// REQUIRES: neither <left> nor <right> is a null pointer
		Clause(std::unique_ptr<Expression>&& left, std::unique_ptr<Expression>&& right,
			Operator_e op, bool isNeg = false);

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
		mutable std::unique_ptr<Expression> leftExpr;							// mutable to allow for the "negate-print-negate"
		mutable std::unique_ptr<Expression> rightExpr;							//   paradigm within the const print function
		Operator_e op;
		bool negated;

		// [Evaluation Helpers]
		// REQUIRES: <op> is the appropriate Boolean operator
		// EFFECTS:  uses the variable assignments described by <assign> to evaluate <this>
		//   and returns the evaluation, either TRUE or FALSE
		bool evaluateAnd(const Assignment& assign) const;
		bool evaluateOr(const Assignment& assign) const;
		bool evaluateImplies(const Assignment& assign) const;
		bool evaluateXor(const Assignment& assign) const;
        bool evaluateIff(const Assignment& assign) const;

		// [Printer Helpers]
		// MODIFIES: <os>
		// REQUIRES: <op> is the appropriate Boolean operator
		// EFFECTS:  prints <this> to <os> without a trailing newline character
		void printAnd(std::ostream& os) const;
		void printOr(std::ostream& os) const;
		void printImplies(std::ostream& os) const;
		void printXor(std::ostream& os) const;
        void printIff(std::ostream& os) const;
};

#endif