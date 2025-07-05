#include "Utility.h"											// for extern const chars
#include <cmath>												// for pow
#include <iostream>												// for istream
#include <string>												// for string

using std::istream;
using std::pow;
using std::string;

const char kNotSymbol = '~';
const char kAndSymbol = '*';
const char kOrSymbol = '+';
const char kImpliesSymbol = '>';
const char kXorSymbol = '^';
const char kIffSymbol = '=';


// return the number of variables needed to fully represent a logic puzzle with
// the given dimensions
size_t variablesNeeded(size_t categories, size_t itemsPerCategory) {
	auto res = pow(static_cast<int>(itemsPerCategory), 2) * categories * (categories - 1) / 2;
	return static_cast<size_t>(res);
}

// read an int
int readInt(istream& is) {
	int i;
	if (!(is >> i)) {
		throw EunomiaException{ "Unable to read integer from input stream" };
	}
	return i;
}

// read string
string readString(istream& is) {
	string s;
	is >> s;
	return s;
}

// read until newline
string readLine(istream& is) {
	string str{};
	while (is.peek() != '\n') {
        auto c = is.get();
        if (str != "" || !isspace(c)) {
            str += c;
        }
	}
    is.get();

    if (str == "") {
        return readLine(is);
    }
    else {
        return str;
    }
}

// constructor w/ C-String
EunomiaException::EunomiaException(const char* msg)
	: msg{ msg } {}

// constructor w/ String
EunomiaException::EunomiaException(const string& msg)
	: msg{ msg.c_str() } {}

// return <this>'s error message
const char* EunomiaException::what() const {
	return msg;
}