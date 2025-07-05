#include "Expression.h"											// for Expression
#include <iostream>												// for ostream
#include <string>												// for string

using std::ostream;
using std::string;


// polymorphic destructor
Expression::~Expression() {}

// print <expr> to <os> via the polymorphic <print> function, then return <os>
ostream& operator<<(ostream& os, const Expression& expr) {
	expr.print(os);
	return os;
}