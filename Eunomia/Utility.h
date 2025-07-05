#ifndef EUNOMIA_UTILITY
#define EUNOMIA_UTILITY

#include <exception>											// for exception
#include <iosfwd>												// for istream
#include <string>												// for string, because aliasing this is hard


enum class Operator_e { And, Or, Implies, Xor, Iff };

enum class VarStatus_e { False, True, Unknown };

extern const char kNotSymbol;
extern const char kAndSymbol;
extern const char kOrSymbol;
extern const char kImpliesSymbol;
extern const char kXorSymbol;
extern const char kIffSymbol;

// REQUIRES: <categories> and <itemsPerCategory> are both at least 2
// EFFECTS:  returns the number of variables required to fully represent a logic puzzle
//   that has <categories> Categories of <itemsPerCategory> items each;
size_t variablesNeeded(size_t categories, size_t itemsPerCategory);

// MODIFIES: <is>, standard input
// EFFECTS:  reads a single integer from <is> and returns it; throws a EunomiaException
//   if an integer cannot be read
int readInt(std::istream& is);

// MODIIFES: <is>, standard input
// EFFECTS:  reads a single string from <is> and returns it
std::string readString(std::istream& is);

// MODIFIES: <is>, standard input
// EFFECTS:  reads from <is> until a newline character is encountered and returns a single
//   string consisting of all the contents
std::string readLine(std::istream& is);

/*
	A EunomiaException is a kind of exception that is raised by the components of Eunomia
	when an error arises due to bad user input as opposed to an underlying logical error.
	EunomiaExceptions are most commonly raised by the Controller or the Model, such as in
	the event that the user enters an unrecognized command or uses the name of a Puzzle
	that does not actually name a Puzzle. The EunomiaException's message will contain
	human-readable information describing what cause the exception to arise.
*/

class EunomiaException : public std::exception {
	public:
		// [C-String Constructor]
		// REQUIRES: <msg> is not a null pointer
		EunomiaException(const char* msg);

		// [String Constructor]
		EunomiaException(const std::string& msg);

		// [Message Accessor]
		// EFFECTS:  returns this exception's error message
		const char* what() const final override;

	private:
		const char* msg;
};

#endif