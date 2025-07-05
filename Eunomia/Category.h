#ifndef EUNOMIA_CATEGORY
#define EUNOMIA_CATEGORY

#include <string>												// for string
#include <vector>												// for vector

/*
	A Category is an immutable collection of items, each of which is represented by a string. These item-
	strings do not necessarily have to be unique (although it is recommended that they are). The item-
	strings are loaded into the Category at construction. Additionally, a Category has a name. The number
	of items contained by a Category can be gleaned via an accessor, as can the Category's name.

	Categories provide a direct index-based accessor for obtaining the individual item-strings. In addition,
	Categories provide begin and end iterators for full range iteration.
*/

class Category {
	public:
		// [Constructor]
		// REQUIRES: <name> is non-empty, there are at least two string in the range
		//   [<itemsBegin>, <itemsEnd>)
		template <typename FwdIter>
		Category(const std::string& name, FwdIter itemsBegin, FwdIter itemsEnd)
			: name{ name }, items{ itemsBegin, itemsEnd } {
		
			assertConstruction();
		}

		// [Name Accessor]
		// EFFECTS:  returns <this>'s name
		const std::string& getName() const;

		// [Size Accessor]
		// EFFECTS:  returns the number of items contained in <this>
		size_t size() const;

		// [Item Accessor]
		// REQUIRES: <index> is non-negative and less than <size()>
		// EFFECTS:  returns the <index>th item of <this>
		const std::string& operator[](int index) const;

		/*
			An ItemIterator is a simple iterator taht can be used to iterate over the
			items in a Category. The item-strings cannot be modified through an
			ItemIterator. Because a Category is nonmodifiable, ItemIterators are never
			invalidated.
		*/
		class ItemIterator {
			public:
				// [Increment Operators]
				// MODIFIES: <this>
				// EFFECTS:  pre- or post-increments <this> to the next item in the
				//   parent Category
				ItemIterator& operator++();
				ItemIterator operator++(int);

				// [Dereference Operator]
				// EFFECTS:  returns the item-string of the item currently "pointed"
				//   to by <this>
				const std::string& operator*() const;

				// [Equality Operators]
				// EFFECTS:  returns TRUE if both <this> and <rhs> are "pointing" to the
				//   same item of the same Category, and returns FALSE otherwise; returns
				//   the opposite for the inequality operator
				bool operator==(const ItemIterator& rhs) const;
				bool operator!=(const ItemIterator& rhs) const;

				friend Category;

			private:
				std::vector<std::string>::const_iterator currentItem;

				// [Constructor]
				ItemIterator(std::vector<std::string>::const_iterator iter);
		};
		using iterator_t = ItemIterator;

		// [Iterators]
		// EFFECTS:  returns an iterator to the first (or "one past the last") item of
		//   <this>
		iterator_t begin() const;
		iterator_t end() const;

	private:
		std::vector<std::string> items;
		std::string name;

		// EFFECTS:  asserts if any of the preconditions of the constructor do not hold true, and
		//   does nothing otherwise; this function has no effect when not built in debug mode
		void assertConstruction() const;
};

#endif