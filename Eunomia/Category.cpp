#include "Category.h"											// for Category
#include <cassert>												// for assert
#include <string>												// for string
#include <vector>												// for vector

using std::string;
using std::vector;


// constructor
Category::ItemIterator::ItemIterator(vector<string>::const_iterator iter)
	: currentItem{ iter } {}

// pre-increment
Category::ItemIterator& Category::ItemIterator::operator++() {
	++currentItem;
	return *this;
}

// post-increment
Category::ItemIterator Category::ItemIterator::operator++(int) {
	auto old = *this;
	++*this;
	return old;
}

// dereference
const string& Category::ItemIterator::operator*() const {
	return *currentItem;
}

// equality operator
bool Category::ItemIterator::operator==(const Category::ItemIterator& rhs) const {
	return (currentItem == rhs.currentItem);
}

// inequality operator
bool Category::ItemIterator::operator!=(const Category::ItemIterator& rhs) const {
	return (currentItem != rhs.currentItem);
}

// assert if: there are fewer than 2 items in <this>
void Category::assertConstruction() const {
	assert(items.size() >= 2);
}

// return <this>'s name
const string& Category::getName() const {
	return name;
}

// return the number of items in <items>
size_t Category::size() const {
	return items.size();
}

// index <items> and return appropriate item-string
const string& Category::operator[](int index) const {
	assert(index >= 0 && index < static_cast<int>(size()));
	return items[index];
}

// return an iterator equivalent to <items.cbegin()>
typename Category::iterator_t Category::begin() const {
	return iterator_t{ items.cbegin() };
}

// return an iterator equivalent to <items.cend()>
typename Category::iterator_t Category::end() const {
	return iterator_t{ items.cend() };
}