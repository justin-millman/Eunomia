#ifndef EUNOMIA_EXPRESSION
#define EUNOMIA_EXPRESSION

#include <iosfwd>												// for ostream

class Assignment;

/*
	An Expression is any nonzero number of variables, possibly negated, grouped together and joined by a
	Boolean operator such as "and" or "xor." There are two possible actions that can be taken on an
	Expression: negation and evaluation. When an Expression is evaluated, an assignment to a set of
	variables is used to determine whether the combination of variables, groupings, and Boolean operators
	results in TRUE or FALSE. When an Expression is negated, an assignment that previously resulted in
	a TRUE evaluation subsequently results in a FALSE evaluation; negating an Expression twice is a
	no-op.

	When an Expression is printed, each variable that is comprises the Expression is converted into its
	string representation, and these string representations are separated either by parentheses to
	indicate groupings or by characters indicating the various Boolean operators.
*/

class Expression {
	public:
		// [Destructor]
		// EFFECTS:  virtually destructs <this>
		virtual ~Expression();

		// [Negator]
		// MODIFIES: <this>
		// EFFECTS:  negates <this>
		virtual void negate() = 0;

		// [Evaluator]
		// REQUIRES: each variable that forms part of <this> is within the scope of <assign>
		// EFFECTS:  uses the variable assignments described by <assign> to evaluate <this>
		//   and returns the evaluation, either TRUE or FALSE
		virtual bool evaluate(const Assignment& assign) const = 0;

		friend std::ostream& operator<<(std::ostream& os, const Expression& expr);

	protected:
		// [Printer]
		// MODIFIES: <os>
		// EFFECTS:  prints <this> to <os> without a trailing newline character
		virtual void print(std::ostream& os) const = 0;
};

// [Stream Output Overload]
// MODIFIES: <os>
// EFFECTS:  prints <expr> to <os> without a trailing newline character
std::ostream& operator<<(std::ostream& os, const Expression& expr);

#endif