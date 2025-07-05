#include "Category.h"											// for Category
#include "Expression.h"											// for Expression
#include "Model.h"												// for Model
#include "Puzzle.h"												// for Puzzle
#include "Utility.h"											// for variables()
#include <algorithm>											// for count_if
#include <cassert>												// for assert
#include <vector>												// for vector
#include <string>												// for string

using std::string;
using std::vector;
using std::count_if;


// constructor
Puzzle::CategoryIterator::CategoryIterator(vector<Category>::const_iterator iter)
	: currentItem{ iter } {}

// pre-increment
Puzzle::CategoryIterator& Puzzle::CategoryIterator::operator++() {
	++currentItem;
	return *this;
}

// post-increment
Puzzle::CategoryIterator Puzzle::CategoryIterator::operator++(int) {
	auto old = *this;
	++*this;
	return old;
}

// dereference
const Category& Puzzle::CategoryIterator::operator*() const {
	return *currentItem;
}

// equality operator
bool Puzzle::CategoryIterator::operator==(const Puzzle::CategoryIterator& rhs) const {
	return (currentItem == rhs.currentItem);
}

// inequality operator
bool Puzzle::CategoryIterator::operator!=(const Puzzle::CategoryIterator& rhs) const {
	return (currentItem != rhs.currentItem);
}

// assert if: one or fewer Categories, Categories are of different sizes
void Puzzle::assertConstruction() const {
	assert(name != "");
	assert(items.size() >= 2);
	assert(count_if(items.cbegin(), items.cend(),
		[&](const auto& cat)->bool {
			return cat.size() == items[0].size();
		}
	) == items.size());
}

// return the name
const string& Puzzle::getName() const {
	return name;
}

// return the description
const string& Puzzle::getDescription() const {
	return description;
}

// return the number of Categories in <items>
size_t Puzzle::numCategories() const {
	return items.size();
}

// return the number of items in each Category
size_t Puzzle::itemsPerCategory() const {
	return items[0].size();
}

// let C be the number of Categories and I be the number of items per Category
// return (I^2) * (C choose 2)
size_t Puzzle::numVariables() const {
	auto cats = items.size();
	auto per = items[0].size();
	return variablesNeeded(cats, per);
}

// broadcast all items and statuses to the Model
void Puzzle::broadcastState() const {
	auto& model = Model::getInstance();

	size_t catDim = items.size();
	size_t itemDim = items[0].size();
	for (size_t category = 0; category < catDim; ++category) {
		for (size_t item = 0; item < itemDim; ++item) {
			model.reportItem(name, category, item, items[category][item]);
		}
	}

	size_t vars = numVariables();
	for (size_t i = 0; i < vars; ++i) {
		model.reportStatus(name, i, logic.getStatus(i));
	}
}

// index <items> are return appropriate Category
const Category& Puzzle::operator[](int index) const {
	assert(index >= 0 && index < static_cast<int>(items.size()));
	return items[index];
}

// return an iterator equivalent to <items.cbegin()>
typename Puzzle::iterator_t Puzzle::begin() const {
	return iterator_t{ items.cbegin() };
}

// return an iterator equivalent to <items.cend()>
typename Puzzle::iterator_t Puzzle::end() const {
	return iterator_t{ items.cend() };
}

// apply <clue> via the internal LogicEngine and return the result of the
// application, then report variable statues to <Model>
bool Puzzle::applyClue(const Expression& clue) {
	bool retVal = logic.evaluateClue(clue);

	auto& model = Model::getInstance();
	size_t vars = numVariables();
	for (size_t i = 0; i < vars; ++i) {
		model.reportStatus(name, i, logic.getStatus(i));
	}

	return retVal;
}

// leverage the internal LogicEngine to determine if <this> has been solved
// or not
bool Puzzle::solved() const {
	return logic.solved();
}