#include "Assignment.h"											// for Assignment
#include "Clause.h"												// for Clause
#include "Utility.h"											// for Operator_e
#include <cassert>												// for assert
#include <iostream>												// for ostream
#include <memory>												// for unique_ptr
#include <string>												// for string
#include <utility>												// for move

using std::ostream;
using std::string;
using std::unique_ptr;
using std::move;


// constructor
Clause::Clause(unique_ptr<Expression>&& left, unique_ptr<Expression>&& right,
	Operator_e op, bool isNeg)
	: leftExpr{ move(left) }, rightExpr{ move(right) }, op{ op }, negated{ isNeg } {

	assert(leftExpr && rightExpr);
}

// negate
void Clause::negate() {
	negated = !negated;
}

// use <assign> to evaluate the left-hand Expression and short-circuit if possible;
// if not, use <assign> to evaluate the right-hand Expression and AND the two results
bool Clause::evaluateAnd(const Assignment& assign) const {
	assert(op == Operator_e::And);

	if (!leftExpr->evaluate(assign)) {									// Not Negated: F and ? --> F   |   Negated: ~F or ~? --> T
		return negated;
	}
	return (rightExpr->evaluate(assign) != negated);					// Not Negated: T and T --> T   |   Negated: ~T or ~F --> T
}

// use <assign> to evaluate the left-hand Expression and short-circuit if possible;
// if not, use <assign> to evaluate the right-hand Expression and OR the two results
bool Clause::evaluateOr(const Assignment& assign) const {
	assert(op == Operator_e::Or);
	
	if (leftExpr->evaluate(assign)) {									// Not Negated: T or ? --> T   |   Negated: ~T and ~? --> F
		return !negated;
	}
	return (rightExpr->evaluate(assign) != negated);					// Not Negated: F or T --> T   |   Negated: ~F and ~F --> T
}

// use <assign> to evaluate the left-hand Expression and short-circuit if possible;
// if not, use <assign> to evaluate the right-hand Expression and apply a logical
// implication to the two results
bool Clause::evaluateImplies(const Assignment& assign) const {
	assert(op == Operator_e::Implies);
	
	if (!leftExpr->evaluate(assign)) {									// Not Negated: F imp. ? --> T   |   Negated: F and ~? --> F
		return !negated;
	}
	return (rightExpr->evaluate(assign) != negated);					// Not Negated: T imp. T --> T   |   Negated: T and ~F --> T
}

// use <assign> to evaluate the left- and right-hand Expressions, then XOR the two
// results (no short circuiting is possible)
bool Clause::evaluateXor(const Assignment& assign) const {
	assert(op == Operator_e::Xor);
	
	bool leftEval = leftExpr->evaluate(assign);
	bool rightEval = rightExpr->evaluate(assign);
	return ((leftEval != rightEval) != negated);						// Not Negated: TF or FT --> T   |   Negated: TT or FF --> T
}

// use <assign> to evaluate the left- and right-hand Expressions, then XOR the two
// results (no short circuiting is possible)
bool Clause::evaluateIff(const Assignment& assign) const {
    assert(op == Operator_e::Iff);

    bool leftEval = leftExpr->evaluate(assign);
    bool rightEval = rightExpr->evaluate(assign);
    return ((leftEval == rightEval) != negated);                        // Not Negated: TT or FF --> T   |   Negated: TF or FT --> F
}

// use <assign> to evaluate left- and right- hand Expressions, then apply <op>
// to the results, short-circuiting if possible
bool Clause::evaluate(const Assignment& assign) const {
	switch (op) {
		case Operator_e::And:
			return evaluateAnd(assign);
		case Operator_e::Or:
			return evaluateOr(assign);
		case Operator_e::Implies:
			return evaluateImplies(assign);
		case Operator_e::Xor:
			return evaluateXor(assign);
        case Operator_e::Iff:
            return evaluateIff(assign);
		default:
			throw;												// this will never be reached; cases are exhaustive for Operator_e
	}
}

// if not negated, print "(<leftExpr> AND <rightExpr>)"
// if negated, print "(NOT<leftExpr> OR NOT<rightExpr>)"
void Clause::printAnd(ostream& os) const {
	assert(op == Operator_e::And);
	
	os << '(';
	if (!negated) {
		os << *leftExpr << ' ' << kAndSymbol << ' ' << *rightExpr;
	}
	else {
		leftExpr->negate();
		rightExpr->negate();

		os << *leftExpr << ' ' << kOrSymbol << ' ' << *rightExpr;

		rightExpr->negate();
		leftExpr->negate();
	}
	os << ')';
}

// if not negated, print "(<leftExpr> OR <rightExpr>)"
// if negated, print "(NOT<leftExpr> AND NOT<rightExpr>)"
void Clause::printOr(ostream& os) const {
	assert(op == Operator_e::Or);

	os << '(';
	if (!negated) {
		os << *leftExpr << ' ' << kOrSymbol << ' ' << *rightExpr;
	}
	else {
		leftExpr->negate();
		rightExpr->negate();

		os << *leftExpr << ' ' << kAndSymbol << ' ' << *rightExpr;

		rightExpr->negate();
		leftExpr->negate();
	}
	os << ')';
}

// if not negated, print "(<leftExpr> IMPLIES <rightExpr>)"
// if negated, print "(<leftExpr> AND NOT<rightExpr>)"
void Clause::printImplies(ostream& os) const {
	assert(op == Operator_e::Implies);

	os << '(';
	if (!negated) {
		os << *leftExpr << ' ' << kImpliesSymbol << ' ' << *rightExpr;
	}
	else {
		rightExpr->negate();
		os << *leftExpr << ' ' << kAndSymbol << ' ' << *rightExpr;
		rightExpr->negate();
	}
	os << ')';
}

// if not negated, print "(<leftExpr> XOR <rightExpr>)"
// if negated, print "((<leftExpr> AND <rightExpr>) OR (NOT<leftExpr> AND NOT<rightExpr>))"
void Clause::printXor(ostream& os) const {
	assert(op == Operator_e::Xor);

	os << '(';
	if (!negated) {
		os << *leftExpr << ' ' << kXorSymbol << ' ' << *rightExpr;
	}
	else {
		os << '(';
		os << *leftExpr << ' ' << kAndSymbol << ' ' << *rightExpr;
		os << ')';
		os << ' ' << kOrSymbol << ' ';

		leftExpr->negate();
		rightExpr->negate();

		os << '(';
		os << *leftExpr << ' ' << kAndSymbol << ' ' << *rightExpr;
		os << ')';

		rightExpr->negate();
		leftExpr->negate();
	}
	os << ')';
}

//
void Clause::printIff(std::ostream& os) const {
    assert(op == Operator_e::Iff);

    os << '(';
    if (negated) {
        os << kNotSymbol << '(';
    }
    os << *leftExpr << ' ' << kIffSymbol << ' ' << *rightExpr;
    if (negated) {
        os << ')';
    }
    os << ')';
}

// print: the left-hand Expression, followed by the appropriate symbol for <op>,
// followed by the right-hand Expression, negating all three pieces if necessary
void Clause::print(std::ostream& os) const {
	switch (op) {
		case Operator_e::And:
			return printAnd(os);
		case Operator_e::Or:
			return printOr(os);
		case Operator_e::Implies:
			return printImplies(os);
		case Operator_e::Xor:
			return printXor(os);
        case Operator_e::Iff:
            return printIff(os);
		default:
			throw;												// this will never be reached; cases are exhaustive for Operator_e
	}
}