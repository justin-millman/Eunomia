#include "Utility.h"											// for VarStatus_e
#include "View.h"												// for View
#include <string>												// for string

using std::string;


// constructor
View::View(const string& name, const string& puzzleName)
	: name{ name }, puzzleName{ puzzleName } {}

// destructor
View::~View() {}

// return the name
const string& View::getName() const {
	return name;
}

// return the puzzle's name that <this> is tied to
const string& View::getPuzzle() const {
	return puzzleName;
}

// if <puzzle> matches <puzzleName>, forward to protected helper; else, ignore
void View::reportStatus(const string& puzzle, int varIndex, VarStatus_e status) {
	if (puzzle != puzzleName) {
		return;
	}
	recordStatus(varIndex, status);
}

// if <puzzle> matches <puzzleName>, forward to protected helper; else, ignore
void View::reportItem(const string& puzzle, int category, int item, const string& itemStr) {
	if (puzzle != puzzleName) {
		return;
	}
	recordItem(category, item, itemStr);
}

// by default, ignore the information (derived classes can override)
void View::recordStatus(int, VarStatus_e) {}

// by default, ignore the information (derived classes can override)
void View::recordItem(int, int, const string&) {}