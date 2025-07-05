#include "EunomiaStream.h"                                      // for eout
#include "SummaryView.h"										// for SummaryView
#include "Utility.h"											// for VarStatus_e, variablesNeeded()
#include "View.h"												// for View
#include <algorithm>											// for remove_if, sort, max
#include <iostream>												// for endl
#include <string>												// for string
#include <unordered_map>										// for unordered_map
#include <vector>												// for vector
#include <utility>												// for move

using std::endl;
using std::string; using std::strlen;
using std::vector; using std::unordered_map;
using std::remove_if; using std::sort; using std::max;
using std::move;

const char* const kDefaultItemStr = "-----";
const char* const kNoGroupsMsg = "There are not yet any groups";
const int kItemPadding = 3;
const int kMinItemLen = 4;
const char kBeginGroup = '{';
const char kEndGroup = '}';
const char kHeaderBorder = '=';

// EFFECTS:  returns TRUE if <vec> has only one element that is not the empty
//   string (even if there are no elements that are the empty string) and
//   returns FALSE otherwise
bool hasOnlyOneItem(const vector<string>& vec);


// load all mappings of variable index ---> two items' indices into <varMap>
void SummaryView::loadVarMap(size_t categories, size_t itemsPerCategory) {
	int nextIdx = 0;
	for (size_t yAxisCat = categories; yAxisCat > 1; --yAxisCat) {							// for each category along the y-axis
		size_t yAxisCatIdx = yAxisCat % categories;											//   to get the first y-axis category as Cat0
		for (size_t yAxisItem = 0; yAxisItem < itemsPerCategory; ++yAxisItem) {				//   for each item in that category
			for (size_t xAxisCat = 1; xAxisCat < yAxisCat; ++xAxisCat) {					//     for each category along the x-axis
				for (size_t xAxisItem = 0; xAxisItem < itemsPerCategory; ++xAxisItem) {		//       for each item in that category
					varMap[nextIdx++] = Var_t{												//         use next var index to create mapping entry
						static_cast<int>(yAxisCatIdx),
						static_cast<int>(yAxisItem),
						static_cast<int>(xAxisCat),
						static_cast<int>(xAxisItem)
					};
				}
			}
		}
	}
}

// constructor
SummaryView::SummaryView(const string& name, const string& puzzleName, size_t categories, size_t itemsPerCategory)
	: View{ name, puzzleName }, items(categories, vector<string>(itemsPerCategory, kDefaultItemStr)),
	itemsPerCategory{ itemsPerCategory } {

	int nodesNeeded = static_cast<int>(categories * itemsPerCategory);				// each item gets its own Node
	for (int i = 0; i < nodesNeeded; ++i) {
		disjointSet.push_back(Node_t{ i, 0 });
	}

	loadVarMap(categories, itemsPerCategory);
}

// find the representative of the Node at index <nodeIdx>, doing path compression
int SummaryView::findRepr(int nodeIdx) const {
	auto reprIdx = disjointSet[nodeIdx].reprIdx;					// get current representative
	if (reprIdx != nodeIdx) {									// if representative is someone else,
		disjointSet[nodeIdx].reprIdx = findRepr(reprIdx);			//   update representative (this is path compression)
	}
	return disjointSet[nodeIdx].reprIdx;
}

// TRUE if only one item is the empty string
bool hasOnlyOneItem(const vector<string>& vec) {
	int count = 0;
	for (const auto& str : vec) {
		if (str != "") {
			++count;
		}
	}
	return (count == 1);
}

// print the table of groups to <eout>
void SummaryView::display() const {
	// calculate number of categories
	const size_t categories = disjointSet.size() / itemsPerCategory;

	// sort items into groups based on representative
	unordered_map<int, int> groupIDs;								// map of representative ---> group #
	vector<vector<string>> groups;									// groups
	const size_t num = disjointSet.size();
	for (size_t i = 0; i < num; ++i) {								// for each item
		auto reprIdx = findRepr(i);									//   find its representative
		auto foundGroup = groupIDs.find(reprIdx);					//   find if a group already exists for that representative
		int cat = i / itemsPerCategory;
		int item = i % itemsPerCategory;
		auto itemStr = items[cat][item];

		if (foundGroup == groupIDs.cend()) {						//   group doesn't exist yet:
			groupIDs[reprIdx] = static_cast<int>(groups.size());	//     create new mapping entry for new group
			vector<string> newGroup(categories);					//     create new group
			newGroup[cat] = itemStr;								//     add item to new group
			groups.push_back(move(newGroup));						//     add new group to collection of groups
		}
		else {														//   group already exists:
			groups[foundGroup->second][cat] = itemStr;				//     add item to existing group
		}
	}

	// remove groups with only one member
	groups.erase(remove_if(groups.begin(), groups.end(), &hasOnlyOneItem), groups.end());

	// sort remaining groups by earliest-filled item
	sort(groups.begin(), groups.end(),
		[](const auto& lhs, const auto& rhs)-> bool {
			const size_t num = lhs.size();
			for (size_t i = 0; i < num; ++i) {
				if (lhs[i] == "" && rhs[i] != "") {					// <lhs> has no item where <rhs> does
					return false;									//   so <rhs> comes first
				}
				else if (lhs[i] != "" && rhs[i] == "") {			// <rhs> has no item where <lhs> does
					return true;									//   so <lhs> comes first
				}
				else if (lhs[i] != "" && rhs[i] != "") {			// both <lhs> and <rhs> have an item
					return (lhs[i] < rhs[i]);						//   so sort alphabetically by that item
				}
			}
			return false;
		}
	);

	// find the length of the longest item to display in each category
	int sumLengths = categories * kMinItemLen;
	vector<int> lengths(categories, kMinItemLen);
	for (const auto& group : groups) {
		for (size_t i = 0; i < categories; ++i) {
			int newMax = max(lengths[i], static_cast<int>(group[i].size()));
			sumLengths += (newMax - lengths[i]);
			lengths[i] = newMax;
		}
	}

	// print header (underborder depends on if there are groups)
	eout << endl << "Summary View: " << getPuzzle() << endl;

	// print groups, one group per row, with items columnized and centered
	if (groups.empty()) {
		eout << string(strlen(kNoGroupsMsg), kHeaderBorder) << endl;
		eout << kNoGroupsMsg << endl;
		return;
	}

	eout << string(sumLengths + (categories * kItemPadding * 2) + 2, kHeaderBorder) << endl;

	for (const auto& group : groups) {
		eout << kBeginGroup;
		for (size_t i = 0; i < categories; ++i) {
			const auto& itemStr = group[i];
			int beforePadding = ((2 * kItemPadding) + lengths[i] - itemStr.length()) / 2;
			int afterPadding = ((2 * kItemPadding) + lengths[i] - itemStr.length()) - beforePadding;
			eout << string(beforePadding, ' ') << itemStr << string(afterPadding, ' ');
		}
		eout << kEndGroup << endl;
	}
}

// put the two Nodes with indices <node1Idx> and <node2Idx> into the same group
// (i.e. make them have the same representative)
void SummaryView::joinNodes(int node1Idx, int node2Idx) {
	auto repr1Idx = findRepr(node1Idx);												// find representatives
	auto repr2Idx = findRepr(node2Idx);

	if (repr1Idx == repr2Idx) {														// same representative; no work to be done
		return;
	}
	else if (disjointSet[repr1Idx].rank < disjointSet[repr2Idx].rank) {				// different representatives; update based on rank
		disjointSet[repr1Idx].reprIdx = repr2Idx;
	}
	else if (disjointSet[repr1Idx].rank > disjointSet[repr2Idx].rank) {
		disjointSet[repr2Idx].reprIdx = repr1Idx;
	}
	else {
		disjointSet[repr2Idx].reprIdx = repr1Idx;
		++disjointSet[repr1Idx].rank;
	}
}

// remember the new status
void SummaryView::recordStatus(int varIndex, VarStatus_e status) {
	if (status == VarStatus_e::True) {
		auto items = varMap[varIndex];
		int item1Idx = items.item1Cat * itemsPerCategory + items.item1ItemIdx;
		int item2Idx = items.item2Cat * itemsPerCategory + items.item2ItemIdx;

		joinNodes(item1Idx, item2Idx);
	}
}

// remember the new item and add a self-represented Node to the <disjointSet>
void SummaryView::recordItem(int category, int item, const string& itemStr) {
	items[category][item] = itemStr;
}