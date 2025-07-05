#include "Assignment.h"											// for Assignment
#include "Expression.h"											// for Expression
#include "LogicEngine.h"										// for LogicEngine
#include "Utility.h"											// for VarStatus_e, variables()
#include <algorithm>											// for count_if, remove_if, next_permutation, copy
#include <cassert>												// for assert
#include <cmath>												// for pow
#include <iterator>												// for back_inserter
#include <numeric>												// for iota
#include <unordered_map>										// for unordered_map
#include <utility>												// move
#include <vector>												// for vector

using std::vector; using std::unordered_map;
using std::count_if; using std::remove_if; using std::copy;
using std::pow;
using std::next_permutation;
using std::iota;
using std::back_inserter;
using std::move;

using PermutationList_t = vector<vector<int>>;

// REQUIRES: <num> is non-negative, <cumulative> is greater than 0
// EFFECTS:  returns <num>! * <cumulative> tail-recursively
constexpr int factorial(int num, int cumulative = 1);

// REQUIRES: <axisIdx> is in the range [0, <categories>), <categories> is at least
//   2, <itemsPer> is at least 2, <cumulative> is non-negative
// EFFECTS:  returns the index of the variable corresponding to the intersection of
//   item 0 in category <axisIdx> and item 0 in category 1 assuming that the variable
//   at the intersection of item <itemsPer> in category <axisIdx> + 1 and item <itemsPer>
//   in category <categories> is <cumulative>
constexpr int sectionStart(int axisIdx, int categories, int itemsPer, int cumulative = 0);

// REQUIRES: <num> is positive
// EFFECTS:  returns a list of all possible permutations of <num> items indexed 0 through
//   <num> - 1
PermutationList_t permutations(int num);

// REQUIRES: <blockSize> is at least 2
// EFFECTS:  returns the Cartesian product of <blocks> lists of all possible permutations of
//   <blockSize> items indexed 0 through <blockSize> - 1
PermutationList_t permutationsCross(int blocks, int blockSize);

// REQUIRES: <permutation> is non-empty, <categories> and <itemsPer> are both at least 2
// EFFECTS:  returns a list of variable indices corresponding to the items of <permutation>,
//   where each ~n~th value is the index of an item from a category that all belong to the
//   same group
vector<int> getVariables(const vector<int>& permutation, int categories, int itemsPer);


// return <num>! * <cumulative>
constexpr int factorial(int num, int cumulative) {
	assert(num >= 0);
	assert(cumulative > 0);

	if (num == 0) {
		return cumulative;
	}
	return factorial(num - 1, cumulative * num);
}

// return the index of the first variable in the row-block where items from category <axisIdx>
// label the individual cell rows
constexpr int sectionStart(int axisIdx, int categories, int itemsPer, int cumulative) {
	assert(axisIdx >= 0 && axisIdx < categories);
	assert(categories >= 2);
	assert(itemsPer >= 2);
	assert(cumulative >= 0);

	if (axisIdx == 0) {
		return cumulative;
	}
	int toAdd = static_cast<int>(pow(itemsPer, 2) * (categories - axisIdx));
	return sectionStart(axisIdx - 1, categories, itemsPer, cumulative + toAdd);
}

// return a list of all possible permutations of <num> items, using indices from 0 to <num> - 1
// to represent the items; use dynamic programming to avoid expensive recalculations
PermutationList_t permutations(int num) {
	assert(num > 0);
	static unordered_map<int, PermutationList_t> precalcs{};				// for dynamic programming

	auto iter = precalcs.find(num);											// find if permutations already calculated
	if (iter != precalcs.cend()) {
		return iter->second;
	}

	vector<int> items(num);													// list of items to be permuted
	iota(items.begin(), items.end(), 0);									// fill in ascending order

	PermutationList_t perms{};												// list of permutations
	perms.reserve(factorial(num));
	do {
		perms.emplace_back(items.cbegin(), items.cend());					// add current permutation as new item in list
	} while (next_permutation(items.begin(), items.end()));					//   as long as there is a next permutation

	precalcs[num] = perms;													// store calculation for later use
	return perms;
}

// return the Cartesian product of <blocks> permutations of <blockSize> items, using dynamic
// programming to avoid expensive recalculations
PermutationList_t permutationsCross(int blocks, int blockSize) {
	assert(blockSize >= 2);
	unordered_map<int, unordered_map<int, PermutationList_t>> precalcs;				// for dynamic programming; block->blockSize->list

	if (blocks == 1) {																// recursive base case
		return permutations(blockSize);
	}

	auto iter = precalcs.find(blocks);
	if (iter != precalcs.cend()) {													// find first based on block count
		auto iter2 = iter->second.find(blockSize);									//   then based on block size
		if (iter2 != iter->second.cend()) {
			return iter2->second;
		}
	}

	auto myPermutations = permutations(blockSize);
	auto oneLessPermutations = permutationsCross(blocks - 1, blockSize);			// recursively build list of permutations for rest
	PermutationList_t cartesianPerms{};
	cartesianPerms.reserve(myPermutations.size() * oneLessPermutations.size());

	for (const auto& perm : myPermutations) {
		for (const auto& otherPerm : oneLessPermutations) {
			cartesianPerms.emplace_back(perm.cbegin(), perm.cend());								// start with this block of permutations
			copy(otherPerm.cbegin(), otherPerm.cend(), back_inserter(cartesianPerms.back()));		//   then add rest to back
		}
	}

	precalcs[blocks][blockSize] = cartesianPerms;									// store for later
	return cartesianPerms;
}

// translate each item in <permutations> into a variable index, where [0,1,1,0] would correspond
// to two groups: one has Cat0Item0 and Cat1Item1, and the second has Cat0Item1 and Cat1Item0
vector<int> getVariables(const vector<int>& permutation, int categories, int itemsPer) {
	assert(!permutation.empty());
	assert(categories >= 2);
	assert(itemsPer >= 2);

	vector<int> variables;
	variables.reserve(itemsPer * categories * (categories - 1) / 2);			// categories CHOOSE 2 big cells, itemsPer variables each
	
	size_t permSize = permutation.size();
	for (size_t i = 0; i < permSize; ++i) {
		int cat = i / itemsPer;
		int item = i % itemsPer;

		int var = (categories - 1) * itemsPer * item +							// calculate the index of the variable for the item intersecion
			itemsPer * cat +
			permutation[i];
		variables.emplace_back(var);
	}

	for (int cat = 1; cat < categories; ++cat) {														// get each intracategory match variable
		for (int item = 0; item < itemsPer; ++item) {
			for (int otherCat = cat + 1; otherCat < categories; ++otherCat) {
				int var = sectionStart(categories - otherCat, categories, itemsPer) +					// shift to first cell of row-block
					permutation[itemsPer * (otherCat - 1) + item] * itemsPer * (otherCat - 1) +			// shift to first cell or row-block row
					itemsPer * (cat - 1) +																// shift to first cell of blocks intersection
					permutation[itemsPer * (cat - 1) + item];											// shift to cell of intersection
				variables.emplace_back(var);
			}
		}
	}
	return variables;
}

// constructor
LogicEngine::LogicEngine(size_t numCategories, size_t itemsPerCategory)
	: variables(variablesNeeded(numCategories, itemsPerCategory), 0) {
	
	assert(numCategories >= 2);
	assert(itemsPerCategory >= 2);

	auto perms = permutationsCross(numCategories - 1, itemsPerCategory);
	assignments.reserve(perms.size());

	size_t vars = variablesNeeded(numCategories, itemsPerCategory);;
	for (const auto& perm : perms) {					// for each permutation = possible solution
		size_t num = perm.size();
		Assignment assign{ vars };

		auto vars = getVariables(perm, numCategories, itemsPerCategory);
		for (auto varIdx : vars) {						// for each of the variables in that solution
			assign.setTrue(varIdx);						//   set to TRUE in the Assignment
			++variables[varIdx];						//   increment the TRUE count
		}
		assignments.push_back(move(assign));			// add to list of viable Assignments
	}
}

// return VarStatus_e::True if the count of the variable at index <varIndex>
// in <variables> is the same as the number of Assignments remaining in
// <assignments>, return VarStatus_e::False if the count of the variable at
// index <varIndex> in <variables> is 0, and return VarStatus_e::Unknown
// otherwise
VarStatus_e LogicEngine::getStatus(int varIndex) const {
	assert(varIndex >= 0 && varIndex < static_cast<int>(variables.size()));

	if (variables[varIndex] == 0) {
		return VarStatus_e::False;
	}
	else if (variables[varIndex] == static_cast<long long>(assignments.size())) {
		return VarStatus_e::True;
	}
	else {
		return VarStatus_e::Unknown;
	}
}

// return TRUE if the count of every variable in <variables> is either 0 or
// the same as the number of Assignments remaining in <assignments>, and
// return FALSE otherwise
bool LogicEngine::solved() const {
	long long allCount = static_cast<long long>(assignments.size());
	int statused = count_if(variables.cbegin(), variables.cend(),
		[allCount](auto num)->bool {
			return (num == 0 || num == allCount);
		}
	);

	return (statused == static_cast<int>(variables.size()));
}

// evaluate <clue> against every Assignment remaining in <assignments> and
// remove those for which <clue> evaluates to FALSE; for each such removed
// Assignment, decrement the counts of all variables set to TRUE by 1;
// return TRUE if <clue> is consistent (i.e. doesn't evaluate to FALSE for
// all active Assignments) and return FALSE otherwise
bool LogicEngine::evaluateClue(const Expression& clue) {
	vector<bool> shouldRemove(assignments.size(), false);				// TRUE if assignments[i] causes FALSE evaluation
	int totalToRemove = 0;

	size_t numAssigns = assignments.size();
	for (size_t i = 0; i < numAssigns; ++i) {
		if (!clue.evaluate(assignments[i])) {							// FALSE evaluation
			shouldRemove[i] = true;										//   so should remove this Assignment
			++totalToRemove;
		}
	}

	if (totalToRemove == numAssigns) {									// would remove everything, indicating logical inconsistency
		return false;
	}

	for (size_t i = 0; i < numAssigns; ++i) {
		if (shouldRemove[i]) {
			auto end = assignments[i].trueEnd();
			for (auto begin = assignments[i].trueBegin(); begin != end; ++begin) {				// decrement counts for TRUE variables
				--variables[*begin];
			}
		}
	}

	int index = 0;
	assignments.erase(remove_if(assignments.begin(), assignments.end(),
		[&index, &shouldRemove](const auto&)->bool {					// ignore the Assignment; it's not important
			return (shouldRemove[index++]);								// remove if should, reference-incrementing index
		}),
	assignments.end());
	return true;
}