#ifndef EUNOMIA_PARSER
#define EUNOMIA_PARSER

#include <iosfwd>												// for ostream
#include <unordered_map>										// for unordered_map
#include <memory>												// for unique_ptr
#include <string>												// for string, because it's a template parameter
#include "Utility.h"											// for EunomiaException

class Expression;

/*
	A Parser is a interpretation class that is responsible for turned strings representing Boolean
	expressions into Expressions that are equivalent. A Parser contains a set of entries that maps
	individual spaceless strings to variables. Parsers provide ways to add these entries, remove
	entries, and determine if a particular string is the key to any entry. A single variable can be
	mapped to by more than one parse-string, but each parse-string can map to only a single variable.

	A Parser throws ParseExceptions when it attempts to perform an improper action. The following is
	a complete list of the improper actions that will cause a Parser to throw a ParseException:
			> adding a parse-string that contains a space
			> adding a parse-string that contains a grouping symbol
			> adding a parse-string that contaisn a Boolean operator
			> removing a parse-string that has not been set
			> parsing a Boolean string that has mismatched grouping symbols (i.e. parentheses)
			> parsing a Boolean string that contains parse-strings that have not been set
			> parsing a Boolean string that contains unrecognized operator symbols

	A Boolean string to be parsed may be flexibly formatted. Whitespace is ignored, and is therefore
	simultaneously not necessary to include and nonperturbing if included in excess. Grouping symbols
	(parentheses, brackets, and braces) may be used as desired so long as they are properly matched. If
	no grouping symbols are utilized, the implicit order of grouping is in pairs from left-to-right;
	no Boolean operator has greater precedence than any other.
*/

class Parser {
	public:
		// [Default Constructor]
		Parser();

		// [Range-Fill Constructor]
		template <typename FwdIter>
		Parser(FwdIter begin, FwdIter end) {
			int idx = 0;
			while (begin != end) {
				addParseString(*begin++, idx++);
			}
		}

		// [String Adder]
		// REQUIRES: <varIdx> is non-negative
		// MODIFIES: <this>
		// EFFECTS:  if <varStr> contains a space or a grouping symbol or a Boolean opeator,
		//   throws a ParseException; otherwise, adds an entry to <this> that maps <varStr>
		//   to the variable with index <varIdx> for parsing, overwriting any existing entry
		//   for <varStr> if necessary
		void addParseString(const std::string& varStr, int varIdx);

		// [String Remover]
		// MODIFIES: <this>
		// EFFECTS:  if there is not currently an entry for <varStr> in <this>, throws a
		//   ParseException; otherwise, removes the entry of <this> that maps <varStr> to
		//   a variable
		void removeParseString(const std::string& varStr);

		// [String Checker]
		// EFFECTS:  returns TRUE if there is an entry for <varStr> in <this>, and returns
		//   FALSE otherwise
		bool isParseString(const std::string& varStr) const;

		// [Parser]
		// EFFECTS:  attempts to parse <expression> using the entries that have been set in
		//   <this> and returns a unique_ptr to an Expression that is equivalent to <expression>;
		//   if <expression> is malformed (unmatched parentheses, unrecognized variable strings,
		//   etc.), throws a ParseException
		//
		// NOTE:     the following grouping symbols are allowed: parentheses (), brackets [],
		//           and braces {}
		std::unique_ptr<Expression> parse(const std::string& expression) const;

		friend std::ostream& operator<<(std::ostream& os, const Parser& parser);

	private:
		std::unordered_map<std::string, int> variableMap;
};


/*
	A ParseException is a kind of EunomiaException that is thrown when an improper
	action is attempted to be taken by a Parser. Examples of improper actions include,
	but are not limited to: parsing an improperly-formatting string, attempting to
	remove a parse-string that isn't set, or setting an illegal parse-string.
*/

class ParseException : public EunomiaException {
	public:
		// [C-String Constructor]
		ParseException(const char* msg);

		// [String Constructor]
		ParseException(const std::string& msg);
};

// prints all the parse-strings of <parser> to <os> in ascending order by variable index,
// or prints a message indicating that there are no parse-strings in <parser>, then returns
// <os>
std::ostream& operator<<(std::ostream& os, const Parser& parser);

#endif