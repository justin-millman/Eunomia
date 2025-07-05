#include "Assignment.h"											// for Assignment
#include "Literal.h"											// for Literal
#include "Utility.h"											// for constant char symbols
#include <cassert>												// for assert
#include <iostream>												// for ostream
#include <string>												// for string

using std::ostream;
using std::string;


// constructor
Literal::Literal(int id, const string& name, bool isNeg)
	: name{ name }, varID{ id }, negated{ isNeg } {

	assert(id >= 0);
	assert(name.length() > 0);
}

// negate
void Literal::negate() {
	negated = !negated;
}

// use <assign> to get assigned value, negate if needed
bool Literal::evaluate(const Assignment& assign) const {
	assert(varID < static_cast<int>(assign.size()));

	bool assignedValue = assign.getAssignedValue(varID);
	return (assignedValue != negated);										// only (assign=T, negated=F) and (assign=F, negated=T) are TRUE
}

// print: negation symbol if negated, followed by the string representation (no newline)
void Literal::print(std::ostream& os) const {
	if (negated) {
		os << kNotSymbol;
	}
	os << name;
}