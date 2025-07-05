#include "Assignment.h"											// for Assignment
#include <algorithm>											// for find
#include <cassert>												// for assert
#include <iterator>												// for distance
#include <vector>												// for vector

using std::find;
using std::distance;
using std::out_of_range;


// constructor
Assignment::VarIterator::VarIterator(const Assignment* assign, bool target, int initialPos)
	: position{ initialPos }, assignment{ assign }, target{ target } {

	assert(assign);
	assert(initialPos >= 0 && initialPos <= static_cast<int>(assign->size()));
}

// pre-increment
Assignment::VarIterator& Assignment::VarIterator::operator++() {
	do {
		++position;
	} while (position != assignment->size() && assignment->getAssignedValue(position) != target);
	return *this;
}

// post-increment
Assignment::VarIterator Assignment::VarIterator::operator++(int) {
	auto old = *this;
	++*this;
	return old;
}

// dereference by returning the position (which is the variable's index)
int Assignment::VarIterator::operator*() const {
	if (position == assignment->size()) {
		throw out_of_range("Assignment iterator not dereferencable");
	}
	return position;
}

// equality operator
bool Assignment::VarIterator::operator==(const Assignment::VarIterator& rhs) const {
	return (position == rhs.position &&
		assignment == rhs.assignment &&
		target == rhs.target);
}

// inequality operator
bool Assignment::VarIterator::operator!=(const Assignment::VarIterator& rhs) const {
	return !(*this == rhs);
}

// constructor
Assignment::Assignment(size_t varCount)
	: truths(varCount, false) {

	assert(varCount > 0);
}

// return the size, which is the number of variables assigned to
size_t Assignment::size() const {
	return truths.size();
}

// index <truths> for the assigned value
bool Assignment::getAssignedValue(int varIndex) const {
	assert(varIndex >= 0 && varIndex < static_cast<int>(truths.size()));
	return truths[varIndex];
}

// change the value of <truths> at given idex to TRUE
void Assignment::setTrue(int varIndex) {
	assert(varIndex >= 0 && varIndex < static_cast<int>(truths.size()));
	truths[varIndex] = true;
}

// change the value of <truths> at given index to FALSE
void Assignment::setFalse(int varIndex) {
	assert(varIndex >= 0 && varIndex < static_cast<int>(truths.size()));
	truths[varIndex] = false;
}

// return an iterator to the first element of <truths> whose value is <target>,
// or "one past the end" if none have the value of <target>
typename Assignment::iterator_t Assignment::makeBeginIterator(bool target) const {
	auto iter = find(truths.cbegin(), truths.cend(), target);
	if (iter == truths.cend()) {
		return iterator_t{ this, target, static_cast<int>(truths.size()) };
	}

	int dist = static_cast<int>(distance(truths.cbegin(), iter));
	return iterator_t{ this, target, dist };
}

// return an iterator to "one past the last" element of <truths>
typename Assignment::iterator_t Assignment::makeEndIterator(bool target) const {
	return iterator_t{ this, target, static_cast<int>(truths.size()) };
}

// return an iterator to the first element of <truths> that is TRUE, or
// "one past the end" if none are TRUE
typename Assignment::iterator_t Assignment::trueBegin() const {
	return makeBeginIterator(true);
}

// return an iterator to "one past the last" element of <truths> that is
// TRUE, or "one past the end" if none are TRUE
typename Assignment::iterator_t Assignment::trueEnd() const {
	return makeEndIterator(true);
}

// return an iterator to the first element of <truths> that is FALSE, or
// "one past the end" if none are FALSE
typename Assignment::iterator_t Assignment::falseBegin() const {
	return makeBeginIterator(false);
}

// return an iterator to "one past the last" element of <truths> that is
// FALSE, or "one past the end" if none are FALSE
typename Assignment::iterator_t Assignment::falseEnd() const {
	return makeEndIterator(false);
}