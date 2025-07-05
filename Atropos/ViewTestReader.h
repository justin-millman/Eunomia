#ifndef ATROPOS_VIEW_READER
#define ATROPOS_VIEW_READER

#include <string>												// for string

extern const char* kRelativeFileLoc;


// REQUIRES: none of <viewName>, <testName>, or <suffix> is a null-pointer
// EFFECTS:  returns the concatenation of <viewName> with <testName> with <suffix>
std::string buildTestName(const char* viewName, const char* testName, const char* suffix);

// REQUIRES: neither <scenario> nor <correct> is a null-pointer, there are properly-
//   formatted text files named <scenario> and <correct>
// EFFECTS:  builds a GridView based on the contents of the text file <scenario> and returns
//   TRUE if the GridView's output matches the contents of the text file <correct>, otherwise
//   returns FALSE
bool testGridView(const std::string& scenario, const std::string& correct);

// REQUIRES: neither <scenario> nor <correct> is a null-pointer, there are properly-
//   formatted text files named <scenario> and <correct>
// EFFECTS:  builds a SummaryView based on the contents of the text file <scenario> and returns
//   TRUE if the SummaryView's output matches the contents of the text file <correct>, otherwise
//   returns FALSE
bool testSummaryView(const std::string& scenario, const std::string& correct);

#endif