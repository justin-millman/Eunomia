#include "Clause.h"												// for Clause
#include "Expression.h"											// for Expression
#include "Literal.h"											// for Literal
#include "Parser.h"												// for Parser
#include "Utility.h"											// for EunomiaException, Operator_e, constant char symbols
#include <algorithm>											// for any_of, sort, find
#include <cassert>												// for assert
#include <cctype>												// for isspace
#include <cmath>												// for abs, max, log10, floor
#include <iomanip>												// for setw
#include <iostream>												// for ostream
#include <memory>												// for unique_ptr, make_unique
#include <stack>												// for stack
#include <string>												// for string
#include <utility>												// for pair, move, get
#include <variant>												// for variant
#include <vector>												// for vector

using std::string; using std::isspace;
using std::vector; using std::pair; using std::stack;
using std::ostream; using std::setw;
using std::unique_ptr; using std::make_unique;
using std::any_of;  using std::find; using std::sort;
using std::abs; using std::max; using std::log10; using std::floor;
using std::move;
using std::variant; using std::get;

const char kOpenGroups[]{ '(', '[', '{' };
const char kCloseGroups[]{ ')', ']', '}' };
const char kOperators[]{ kAndSymbol, kOrSymbol, kImpliesSymbol, kXorSymbol, kIffSymbol };
const size_t kNumOpenGroups = sizeof(kOpenGroups) / sizeof(char);
const size_t kNumCloseGroups = sizeof(kCloseGroups) / sizeof(char);
const size_t kNumOperators = sizeof(kOperators) / sizeof(char);
const char* kBadGroupLocMsg = "Error: Grouping symbol unexpectedly encountered while parsing";
const char* kBadOperLocMsg = "Error: Boolean operator unexpectedly encountered while parsing";
const char* kBadNegLocMsg = "Error: Negation operator unexpectedly encountered while parsing";
const char* kBadVarLocMsg = "Error: Variable parse-string unexpectedly encountered while parsing";
const char* kUnrecVarMsg = "Error: Unrecognized variable string encountered while parsing";
const char* kUnmatchGroupMsg = "Error: Groupings not closed or mismatched";
const char* kEarlyEndMsg = "Error: End-of-string unexpectedly met while parsing";

using striter_t = string::const_iterator;
enum class State_e { StartCommon, CloseGroup, PreVariable, PostVariable, End };

using TokenValue_t = variant<unique_ptr<Expression>, Operator_e, char>;
enum class TokenType_e { Expression, Operator, Negation, OpenGroup };
struct Token {
	TokenType_e type;
	TokenValue_t value;
};

// REQUIRES: <charArray> is not a null-pointer, <count> is positive
// EFFECTS:  returns TRUE if <probe> is one of the characters in <charArray>, and returns FALSE
//   otherwise
bool isInGroup(char probe, const char* charArray, size_t count);

// EFFECTS:  returns TRUE if <probe> is a reserved character and returns FALSE otherwise
bool isReserved(char probe);

// REQUIRES: <opChar> is an element of <kOperators>
// EFFECTS:  returns the Operator_e that is represented by the <opChar>
Operator_e getOperator(char opChar);

// REQUIRES: <closeSymbol> is an element of <kCloseGroups>
// MODIFIES: <tokens>
// EFFECTS:  pops Tokens off of <tokens> until an OpenGroup token is encountered, replacing the
//   popped-off Tokens with an equivalent Expression, then pops off the OpenGroup token; if the
//   value of the OpenGroup token is not <openTarget>, throws a ParseException
void unwindStack(stack<Token>& tokens, char closeSymbol);

// REQUIRES: <rightExpr> is not a null-pointer
// MODIFIES: <tokens>
// EFFECTS:  constructs a new Token corresponding to the Literal <varIdx> with display string
//   <varStr>, then looks at the top of <tokens> and negates that Literal if the top is a
//   Negation Token or builds a Clause if the top is a Operator token, then pushes the Literal
//   onto <tokens>
void pushExpression(stack<Token>& tokens, unique_ptr<Expression>&& rightExpr);

// REQUIRES: <iter> is valid (i.e. dereferenceable)
// MODIFIES: <iter>
// EFFECTS:  advances <iter> to the first non-whitespace character after its current position
void skipWhitespace(striter_t& iter, const striter_t end);

// REQUIRES: <iter> is valid (i.e. dereferenceable)
// MODIFIES: <iter>
// EFFECTS:  builds a string consisting of the character of <iter> from its current position to
//   the first non-restricted character (grouping symbol, operator, or negation symbol) or
//   whitespace, then returns that string; <iter> gets advanced to the character that caused the
//   string building to terminate
string extractVariable(striter_t& iter, const striter_t end);

// REQUIRES: <pos> and <end> are iterators to the same string and <pos> points to an earlier
//   position in that string than <end>
// MODIFIES: <tokens>, <pos>
// EFFECTS:  parses characters from <pos> as if the last thing parsed was nothing (i.e. just
//   starting to parse), an operator, an open-group symbol, or a negation symbol, adding additional
//   Tokens to <tokens> as appropriate; returns the next state for the Parser FSA
State_e parseStartCommon(stack<Token>& tokens, striter_t& pos, const striter_t end);

// REQUIRES: <pos> and <end> are iterators to the same string and <pos> points to an earlier
//   position in that string than <end>
// MODIFIES: <tokens>, <pos>
// EFFECTS:  parses characters from <pos> as if the last thing parsed was a close-group symbol,
//   adding additional Tokens to <tokens> as appropriate; returns the next state for the Parser
//   FSA
State_e parseAfterClose(stack<Token>& tokens, striter_t& pos, const striter_t end);

// REQUIRES: <pos> and <end> are iterators to the same string and <pos> points to an earlier
//   position in that string than <end>
// MODIFIES: <tokens>, <pos>
// EFFECTS:  parses characters from <pos> as if the last thing parsed was a variable's parse-
//   string, adding additional Tokens to <tokens> as appropriate; returns the next state for the
//   Parser FSA
State_e parseAfterVariable(stack<Token>& tokens, striter_t& pos, const striter_t end);


// return TRUE if <probe> is in <charArray> and FALSE otherwise
bool isInGroup(char probe, const char* charArray, size_t count) {
	assert(charArray);
	assert(count > 0);

	return (find(charArray, charArray + count, probe) != charArray + count);
}

// return TRUE if <probe> is a reserved characters and FALSE otherwise
bool isReserved(char probe) {
	if (isInGroup(probe, kOpenGroups, sizeof(kOpenGroups) / sizeof(char))) {
		return true;
	}
	else if (isInGroup(probe, kCloseGroups, sizeof(kCloseGroups) / sizeof(char))) {
		return true;
	}
	else if (isInGroup(probe, kOperators, sizeof(kOperators) / sizeof(char))) {
		return true;
	}
	else if (probe == kNotSymbol) {
		return true;
	}
	else if (isspace(probe)) {
		return true;
	}
	else {
		return false;
	}
}

// return the equivalent Operator_e to <opChar>
Operator_e getOperator(char opChar) {
	assert(find(kOperators, kOperators + kNumOperators, opChar)
		!= kOperators + kNumOperators);

	if (opChar == kAndSymbol) {
		return Operator_e::And;
	}
	else if (opChar == kOrSymbol) {
		return Operator_e::Or;
	}
	else if (opChar == kImpliesSymbol) {
		return Operator_e::Implies;
	}
	else if (opChar == kXorSymbol) {
		return Operator_e::Xor;
	}
    else if (opChar == kIffSymbol) {
        return Operator_e::Iff;
    }
	else {												// should never get never; clauses are exhaustive
		throw;
	}
}

// constructor w/ C-String
ParseException::ParseException(const char* msg)
	: EunomiaException{ msg } {}

// constructor w/ String
ParseException::ParseException(const string& msg)
	: EunomiaException{ msg } {}

// default constructor
Parser::Parser() {}

// add or overwrite the parse entry for <parsedValue>, or throw a ParseException
// if <parsedValue> contains a space or grouping symbol or Boolean operator
void Parser::addParseString(const string& parsedValue, int varIdx) {
	assert(varIdx >= 0);
	
	if (any_of(parsedValue.cbegin(), parsedValue.cend(), isReserved)) {
		throw ParseException{ "Reserved character found in parse-string" };
	}
	variableMap[parsedValue] = varIdx;
}

// remove the parse entry for <parsedValue>, or throw a ParseException if there
// isn't one
void Parser::removeParseString(const string& parsedValue) {
	auto iter = variableMap.find(parsedValue);
	if (iter == variableMap.cend()) {
		throw ParseException{ "Parse-string not found" };
	}
	variableMap.erase(iter);
}

// return TRUE if there is a parse entry for <parsedValue>, return FALSE otherwise
bool Parser::isParseString(const string& parsedValue) const {
	return (variableMap.find(parsedValue) != variableMap.cend());
}

// pop Expression Token off <tokens>, then ensure that next Token is an OpenGroup
// token with value of <openTarget>
void unwindStack(stack<Token>& tokens, char closeSymbol) {
	assert(find(kCloseGroups, kCloseGroups + kNumCloseGroups, closeSymbol)
		!= kCloseGroups + kNumCloseGroups);
	
	if (tokens.empty() || tokens.top().type != TokenType_e::Expression) {		// Expression/Variable must come before close group symbol
		throw ParseException{ kBadGroupLocMsg };
	}

	auto expr = move(get<0>(tokens.top().value));
	tokens.pop();

	int idx = find(kCloseGroups, kCloseGroups + kNumCloseGroups,
		closeSymbol) - kCloseGroups;
	char openTarget = kOpenGroups[idx];

	if (tokens.empty() || tokens.top().type != TokenType_e::OpenGroup) {		// back-to-back Expressions will get folded into one, so a
		throw ParseException{ kBadGroupLocMsg };								//   close group symbol should follow an Expression that followed
	}																			//   the appropriate open group symbol
	else if (get<char>(tokens.top().value) != openTarget) {
		throw ParseException{ kUnmatchGroupMsg };
	}
	tokens.pop();

	pushExpression(tokens, move(expr));
}

// push a new Token corresponding to the Literal with index <varIdx> and the string
// representation <varStr> to <tokens>, then apply any negation and clause-ification
// by looking at the top of <tokens>
void pushExpression(stack<Token>& tokens, unique_ptr<Expression>&& rightExpr) {
	assert(rightExpr);
	
	while (!tokens.empty() && tokens.top().type == TokenType_e::Negation) {					// negate as long as needed
		rightExpr->negate();
		tokens.pop();
	}

	if (!tokens.empty() && tokens.top().type == TokenType_e::Operator) {					// if previous is an Operator, then two-previous
		Operator_e op = get<Operator_e>(tokens.top().value);								//   must be another Expression (otherwise the
		tokens.pop();																		//   Operator would have been flagged as invalid)

		auto leftExpr = move(get<0>(tokens.top().value));
		tokens.pop();

		auto clause = make_unique<Clause>(move(leftExpr), move(rightExpr), op);				// so turn the two Expressions and Operator into a
		pushExpression(tokens, move(clause));												//   Clause and recurse in case there's no grouping
	}
	else {
		tokens.push(Token{ TokenType_e::Expression, move(rightExpr) });						// when out of Operators, push Expression onto stack
	}
}

// skip all whitespace, then return a pointer to the first character that is something
// else
void skipWhitespace(striter_t& iter, const striter_t end) {
	while (iter != end && isspace(*iter)) {
		++iter;
	}
}

// build a string from <iter> character-by-character until non-variable-character is
// encountered
string extractVariable(striter_t& iter, const striter_t end) {
	string retVal{};
	while (iter != end && !isspace(*iter) && !isReserved(*iter)) {
		retVal += *iter++;
	}
	return retVal;
}

// assume that the last thing parsed by the Parser was either nothing (just starting
// to parse), an open-group symbol, a negation symbol, or an operator
State_e parseStartCommon(stack<Token>& tokens, striter_t& pos, const striter_t end) {
	skipWhitespace(pos, end);												// skip all leading whitespace
	if (pos == end) {														// done if no more characters to parse
		return State_e::End;
	}

	if (isInGroup(*pos, kOpenGroups, kNumOpenGroups)) {						// parse open-group symbol
		tokens.push(Token{ TokenType_e::OpenGroup, *pos++ });				//   tokenize and remove from stream
		return State_e::StartCommon;
	}
	else if (isInGroup(*pos, kCloseGroups, kNumCloseGroups)) {				// parse close-group symbol
		throw ParseException{ kBadGroupLocMsg };							//   throw exception
	}
	else if (isInGroup(*pos, kOperators, kNumOperators)) {					// parse operator
		throw ParseException{ kBadOperLocMsg };								//   throw exception
	}
	else if (*pos == kNotSymbol) {											// parse negation symbol
		tokens.push(Token{ TokenType_e::Negation, kNotSymbol });			//   tokenize and remove from stream
		++pos;
		return State_e::StartCommon;
	}
	else {																	// parse first character of variable parse-string
		return State_e::PreVariable;										//   leave in stream, let Parser extract and match
	}
}

// assume that the last thing parsed by the Parser was a close-group symbol
State_e parseAfterClose(stack<Token>& tokens, striter_t& pos, const striter_t end) {
	skipWhitespace(pos, end);												// skip all leading whitespace
	if (pos == end) {														// done if no more characters to parse
		return State_e::End;
	}

	if (isInGroup(*pos, kOpenGroups, kNumOpenGroups)) {						// parse open-group symbol
		throw ParseException{ kBadGroupLocMsg };							//   throw exception
	}
	else if (isInGroup(*pos, kCloseGroups, kNumCloseGroups)) {				// parse close-group symbol
		unwindStack(tokens, *pos++);										//    unwind stack and remove from stream
		return State_e::CloseGroup;
	}
	else if (isInGroup(*pos, kOperators, kNumOperators)) {					// parse operator
		Operator_e op = getOperator(*pos++);								//   determine what the operator is
		tokens.push(Token{ TokenType_e::Operator, op });					//   tokenize and remove from stream
		return State_e::StartCommon;
	}
	else if (*pos == kNotSymbol) {											// parse negation symbol
		throw ParseException{ kBadNegLocMsg };								//   throw exception
	}
	else {																	// parse first character of variable parse-string
		throw ParseException{ kBadVarLocMsg };								//   throw exception
	}
}

// assume that the last thing parsed was a complete variable parse-string
State_e parseAfterVariable(stack<Token>& tokens, striter_t& pos, const striter_t end) {
	skipWhitespace(pos, end);												// skip all leading whitespace
	if (pos == end) {														// done if no more characters to parse
		return State_e::End;
	}

	if (isInGroup(*pos, kOpenGroups, kNumOpenGroups)) {						// parse open-group character
		throw ParseException{ kBadGroupLocMsg };							//   throw exception
	}
	else if (isInGroup(*pos, kCloseGroups, kNumCloseGroups)) {				// parse close-group character
		unwindStack(tokens, *pos++);										//   unwind stack and remove from stream
		return State_e::CloseGroup;
	}
	else if (isInGroup(*pos, kOperators, kNumOperators)) {					// parse operator
		Operator_e op = getOperator(*pos++);								//   determine what the operator is
		tokens.push(Token{ TokenType_e::Operator, op });					//   tokenize and remove from stream
		return State_e::StartCommon;
	}
	else if (*pos == kNotSymbol) {											// parse negation symbol
		throw ParseException{ kBadNegLocMsg };								//   throw exception
	}
	else {																	// shouldn't ever get here - would have been part of
		throw ParseException{ kBadGroupLocMsg };							// last-read parse-string
	}
}

// parse <text> into an equivalent Boolean expression, or throw a ParseException if
// <text> is malformed (i.e. unmatched parentheses, unrecognized string, etc.)
std::unique_ptr<Expression> Parser::parse(const std::string& text) const {
	stack<Token> parseTokens{};

	auto end = text.cend();
	auto begin = text.cbegin();
	State_e state = State_e::StartCommon;
	while (begin != end) {
		switch (state) {
			case State_e::StartCommon:
				state = parseStartCommon(parseTokens, begin, end);
				break;
			case State_e::CloseGroup:
				state = parseAfterClose(parseTokens, begin, end);
				break;
			case State_e::PostVariable:
				state = parseAfterVariable(parseTokens, begin, end);
				break;
			case State_e::PreVariable:
				{
					auto var = extractVariable(begin, end);							// extract the variable parse-string
					try {
						auto lit = make_unique<Literal>(variableMap.at(var), var);	// create appropriate Literal
						pushExpression(parseTokens, move(lit));						// push onto stack, and unwind if needed
					}
					catch (...) {													// .at() because [] is non-const; will throw if not found
						throw ParseException{ kUnrecVarMsg };
					}
				}
				state = State_e::PostVariable;
				break;
			case State_e::End:
			default:																// should never get here; only reason for End state is out of
				throw ParseException{ "" };											// characters to parse, which would exit while loop
		}
	}

	if (parseTokens.top().type != TokenType_e::Expression) {			// e.g. ended with an operator or an operator
		throw ParseException{ kEarlyEndMsg };
	}
	if (parseTokens.size() != 1) {										// must be unmatched open-group characters
		throw ParseException{ kUnmatchGroupMsg };
	}

	auto expr = move(get<0>(parseTokens.top().value));
	return expr;
}

// print the unrestricted parse-strings of <parser> and their corresponding variable
// IDs to <os> in ascending order by variable index
ostream& operator<<(ostream& os, const Parser& parser) {
	using Entry_t = pair<int, string>;

	if (parser.variableMap.empty()) {
		os << "There are no parse-strings set in this Parser\n";
		return os;
	}
	
	int maxVar = 0;																	// track max variable for spacing
	vector<Entry_t> entries;
	entries.reserve(parser.variableMap.size());
	for (const auto& pair : parser.variableMap) {
		entries.emplace_back(pair.second, pair.first);
		maxVar = max(maxVar, pair.second);
	}
	sort(entries.begin(), entries.end());

	int width = static_cast<int>(floor(log10(maxVar))) + 1;							// number of digits in largest variable
	for (const auto& entry : entries) {
		os << setw(width) << entry.first << ": " << entry.second << "\n";
	}

	return os;
}