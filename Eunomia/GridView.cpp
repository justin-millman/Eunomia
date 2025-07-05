#include "EunomiaStream.h"                                      // for eout
#include "GridView.h"											// for GridView
#include "Utility.h"											// for VarStatus_e
#include "View.h"												// for View
#include <algorithm>											// for max
#include <cmath>												// for pow
#include <iomanip>												// for setw
#include <iostream>												// for endl
#include <unordered_map>										// for unordered_map
#include <string>												// for string
#include <vector>												// for vector

using std::string;
using std::vector; using std::unordered_map;
using std::endl; using std::setw;
using std::max; using std::pow;

const char* const kDefaultItemStr = "-----";
const char kTrueIndic = '+';
const char kFalseIndic = '\u00D7';           // ×
const char kUnknownIndic = ' ';
const char kHardBorder = '|';
const char kSoftBorder = '\u00B7';           // ·
const char kUnderBorder = '-';
const char kHeaderBorder = '=';


// constructor
GridView::GridView(const string& name, const string& puzzleName,
	size_t categories, size_t itemsPerCategory)
	: View{ name, puzzleName }, items(categories, vector<string>(itemsPerCategory, kDefaultItemStr)),
	  statuses(variablesNeeded(categories, itemsPerCategory), VarStatus_e::Unknown) {}

// print the logic puzzle grid to <eout>
void GridView::display() const {
	// size statistics
	const size_t categories = items.size();
	const size_t itemsPer = items[0].size();

	// calculate axis label sizes
	size_t vertHeight = 0;														// length of longest item in Categories 2 through END
	size_t horzWidth = 0;														// length of longest item in Categories excluding 2
	for (size_t cat = 0; cat < categories; ++cat) {
		for (size_t item = 0; item < itemsPer; ++item) {
			size_t len = items[cat][item].length();
			if (cat != 1) {
				horzWidth = max(horzWidth, len);
			}
			if (cat != 0) {
				vertHeight = max(vertHeight, len);
			}
		}
	}
    ++horzWidth;

	// print header
	eout << endl << "Grid View: " << getPuzzle() << endl;
	int totalWidth = horzWidth + 1 + categories + (3 * (categories - 1) * itemsPer) + ((categories - 1) * (itemsPer - 1));
	eout << string(totalWidth, kHeaderBorder) << endl << endl;

	// print labels along x-axis (top)
	const string frontPadding(horzWidth, ' ');									// length of the y-axis labels, padded out in front
	bool printedCharLast = false;												// whether or not the previous label slot has a character
	for (size_t labelPos = 0; labelPos < vertHeight; ++labelPos) {				// for each position in the vertical label (some word has letter at all)
		eout << frontPadding;
		for (size_t cat = 1; cat < categories; ++cat) {							//   for each category other than the first, in order
			for (size_t item = 0; item < itemsPer; ++item) {					//     for each item in the category
				const string& itemStr = items[cat][item];
				int letter = labelPos - vertHeight + itemStr.size();			//       which letter should be extracted for this label slot
				char disp = (letter < 0 ? ' ' : itemStr[letter]);				//       space if not yet reached beginning of item string

				char preSym = kSoftBorder;									    //       choose what border goes before label character
				if (item == 0) {
					preSym = kHardBorder;
				}
				else if (!printedCharLast || letter < 0) {
					preSym = ' ';
				}

				eout << ' ' << preSym << ' ' << disp;
				printedCharLast = (letter >= 0);
			}
		}
		eout << ' ' << kHardBorder << endl;
		printedCharLast = false;
	}

	size_t numDashes = horzWidth + (4 * itemsPer * (categories - 1)) + 2;
	eout << string(numDashes, kUnderBorder) << endl;

	// print rows of table, y-axis labels and grid cells
	const string labelUnderBorder(horzWidth + 1, kSoftBorder);							// border that goes beneath y-axis labels of the same category
	const string secBorder(4 * itemsPer - 1, kSoftBorder);								// border that goes under each grid section to sep. categories
	auto oldWidth = eout.width();
	int varIdx = 0;
	for (size_t catIdx = categories; catIdx > 1; --catIdx) {							// for each category (except for Category #1)
		size_t cat = catIdx % categories;												//   to get the first category to be category 0, then reverse
		for (size_t item = 0; item < itemsPer; ++item) {								//   for each item in the category
			const string& itemStr = items[cat][item];
            eout << string(horzWidth - itemStr.size(), ' ') << itemStr;
			for (size_t otherCat = 1; otherCat < catIdx; ++otherCat) {					//     for each paired category along the y-axis
				for (size_t otherItem = 0; otherItem < itemsPer; ++otherItem) {			//       for each item in the other category
					auto status = statuses[varIdx++];									//         status of the intersection variable

					char preSym = (otherItem == 0 ? kHardBorder : kSoftBorder);		    //         symbol to separate from previous cell
					char statusSym = kUnknownIndic;									    //         symbol to go in the cell
					if (status == VarStatus_e::True) {
						statusSym = kTrueIndic;
					}
					else if (status == VarStatus_e::False) {
						statusSym = kFalseIndic;
					}

					eout << ' ' << preSym << ' ' << statusSym;
				}
			}
			eout << ' ' << kHardBorder << endl;

			if (item != itemsPer - 1) {													// print soft border under each non-category-final row
				eout << labelUnderBorder << kHardBorder;
				for (size_t c = 1; c < catIdx; ++c) {
					eout << secBorder << kHardBorder;
				}
				eout << endl;
			}
		}

		size_t numDashes = horzWidth + (4 * itemsPer * (catIdx - 1)) + 2;
		eout << string(numDashes, kUnderBorder) << endl;
	}
}

// remember the new status
void GridView::recordStatus(int varIndex, VarStatus_e status) {
	statuses[varIndex] = status;
}

// remember the new item
void GridView::recordItem(int category, int item, const string& itemStr) {
	items[category][item] = itemStr;
}