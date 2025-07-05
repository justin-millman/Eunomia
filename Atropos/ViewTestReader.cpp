#include "stdafx.h"												// because the framework requires this header file
#include "../Eunomia/GridView.h"								// for GridView
#include "../Eunomia/SummaryView.h"								// for SummaryView
#include "../Eunomia/Utility.h"									// for VarStatus_e
#include "../Eunomia/View.h"									// for View-inherited functions
#include "ViewTestReader.h"										// for testing functions
#include <fstream>												// for ifstream
#include <iostream>												// for cout
#include <sstream>												// for stringstream, strbuf
#include <string>												// for string, strlen, strcat

using std::string; using std::strlen; using std::strcat;
using std::ifstream;
using std::cout; using std::endl;
using std::stringstream; using std::streambuf;

const char* kRelativeFileLoc = "../Atropos/ViewTestFiles/";
const char* kFileSuffix = ".txt";
const string kVName = "TestView";
const string kPName = "Test Puzzle";

// REQUIRES: neither <scenario> nor <correct> is a null-pointer, there are properly-
//   formatted text files named <scenario> and <correct>
// EFFECTS:  builds a View of type <Vw> based on the contents of the text file <scenario> and
//   returns TRUE if the View's output matches the contents of the text file <correct>, otherwise
//   returns FALSE
template <typename Vw>
bool testView(const std::string& scenario, const std::string& correct);


// concatenates <viewName> + <testName> + <suffix>
std::string buildTestName(const char* viewName, const char* testName, const char* suffix) {
	string concat{ kRelativeFileLoc };
	concat += viewName;
	concat += testName;
	concat += suffix;
	concat += kFileSuffix;
	return concat;
}

// test a View of type <Vw> with the contents of <scenario> against the contents of
// <correct>
template <typename Vw>
bool testView(const std::string& scenario, const std::string& correct) {
	ifstream scenarioIn{ scenario };										// open files
	ifstream correctIn{ correct };

	size_t categories;
	size_t itemsPer;
	scenarioIn >> categories >> itemsPer;
	Vw view{ kVName, kPName, categories, itemsPer };						// construct View
	for (size_t cat = 0; cat < categories; ++cat) {							// read in all the item strings
		for (size_t item = 0; item < itemsPer; ++item) {
			string itemStr{};
			scenarioIn >> itemStr;
			view.reportItem(kPName, cat, item, itemStr);
		}
	}
	int varIdx;
	while (scenarioIn >> varIdx) {											// read in all the variable assignments
		char val;
		scenarioIn >> val;
		view.reportStatus(kPName, varIdx, (val == 'T' ? VarStatus_e::True : VarStatus_e::False));
	}

	string correctStr{};
	stringstream correctInBuf{};
	correctInBuf << correctIn.rdbuf();
	correctStr = correctInBuf.str();

	stringstream coutRedirect{};
	streambuf* old = cout.rdbuf(coutRedirect.rdbuf());						// redirect standard output to a buffer
	view.display();															// display View content (prints to cout)
	string actualStr = coutRedirect.str();									// extract contents of View display
	cout.rdbuf(old);														// un-redirect cout

	return (correctStr == actualStr);
}

// test a GridView with the contents of <scenario> against the contents of <correct>
bool testGridView(const std::string& scenario, const std::string& correct) {
	return testView<GridView>(scenario, correct);
}

// test a SummaryView with the contents of <secnario> against the contents of <correct>
bool testSummaryView(const std::string& scenario, const std::string& correct) {
	return testView<SummaryView>(scenario, correct);
}