#ifndef EUNOMIA_PUZZLE
#define EUNOMIA_PUZZLE

#include <iterator>												// for distance
#include <string>												// for string
#include <vector>												// for vector
#include "Category.h"											// for Category
#include "LogicEngine.h"										// for LogicEngine

class Expression;

/*
	A Puzzle is a logic puzzle, which consists of several equally-sized Categories of items. A Puzzle
	has a name, which is akin to a title in a book-printed Puzzle, and a description, which may be
	any additional information including, but not limited to, directions and/or a situational summary.
	Both of these pieces of data are accessbile via accessors.
	
	The goal is to determine which combinations of items, one from each Category, belong to the same
	group; each item must belong to exactly one group. Solving the Puzzle is accomplished by applying
	clues --  Boolean statements of fact -- to the Puzzle and determining which items belong in which
	groups. Puzzles provide an accessor for determining if the Puzzle has been solved.

	The number of Categories, as well as the total number of variables (possible pairs of 2 items that
	may be in the same group, probably with other items) needed to represent the Puzzle. Additionally,
	the Categories that make up the Puzzle are accessible either via an indexer or by range iteration.

	A Puzzle can be told, at any time, to broadcast its current state to the Model. When it does so, it
	reports the names of every item and the status of every variable.
*/

class Puzzle {
	public:
		// [Constructor]
		// REQUIRES: <name> is non-empty, there are at least two Categories in the range
		//   [<catBegin>, <catEnd>), each Category in the range [<catBegin>, <catEnd>)
		//   has the same size
		template <typename FwdIter>
		Puzzle(const std::string& name, const std::string& desc, FwdIter catBegin, FwdIter catEnd)
			: logic{ std::distance(catBegin, catEnd), catBegin->size() }, items(catBegin, catEnd),
			name{ name }, description{ desc } {
		
			assertConstruction();
		}

		// [Name and Description Accessors]
		// EFFECTS:  returns <this>'s name or description
		const std::string& getName() const;
		const std::string& getDescription() const;

		// [Size Accessors]
		// EFFECTS:  returns the number of Categories of <this>, the number of items in each
		//   of those Categories, or the total number of variables needed to fully represent
		//   this
		size_t numCategories() const;
		size_t itemsPerCategory() const;
		size_t numVariables() const;

		// [State Broadcaster]
		// MODIFIES: <Model> Singleton instance
		// EFFECTS:  reports all possible current information to the Model
		void broadcastState() const;

		// [Category Accessor]
		// REQUIRES: <index> is non-negative and less than <numCategories()>
		// EFFECTS:  returns the <index>th Category of <this>
		const Category& operator[](int index) const;

		/*
			A CategoryIterator is a simple iterator taht can be used to iterate over the
			Categories in a Puzzle. The Categories cannot be modified through a
			CategoryIterator. Because the Categories of a Puzzle are nonmodifiable,
			CategoryIterators are never invalidated.
		*/
		class CategoryIterator {
			public:
				// [Increment Operators]
				// MODIFIES: <this>
				// EFFECTS:  pre- or post-increments <this> to the next Category in
				//   the parent Puzzle
				CategoryIterator& operator++();
				CategoryIterator operator++(int);

				// [Dereference Operator]
				// EFFECTS:  returns the Category currently "pointed" to by <this>
				const Category& operator*() const;

				// [Equality Operators]
				// EFFECTS:  returns TRUE if both <this> and <rhs> are "pointing" to the
				//   same Category of the same Puzzle, and returns FALSE otherwise; returns
				//   the opposite for the inequality operator
				bool operator==(const CategoryIterator& rhs) const;
				bool operator!=(const CategoryIterator& rhs) const;

				friend Puzzle;

			private:
				std::vector<Category>::const_iterator currentItem;

				// [Constructor]
				CategoryIterator(std::vector<Category>::const_iterator iter);
		};
		using iterator_t = CategoryIterator;

		// [Iterators]
		// EFFECTS:  returns an iterator to the first (or "one past the last") Category of
		//   <this>
		iterator_t begin() const;
		iterator_t end() const;

		// [Clue Applier]
		// REQUIRES: each variable that forms part of <clue> is a variable that is part of <this>,
		//   meaning that each variable's ID is less than <numVariables()>
		// MODIFIES: <this>, <Model> Singleton instance
		// EFFECTS:  applies <clue> to <this>, then if <clue> was consistent (that is, if <clue> did
		//   not contradict any previously-applied clue) broadcasts any status updates to the <Model>
		//   and returns TRUE; otherwise, returns FALSE
		bool applyClue(const Expression& clue);

		// [Solved Checker]
		// EFFECTS:  returns TRUE if and only if every variable of <this> has a status of "guaranteed
		//   to be true" or "guaranteed to be false," indicating whether or not <this> has been totally
		//   solved
		bool solved() const;

	private:
		LogicEngine logic;
		std::vector<Category> items;
		std::string name;
		std::string description;

		// EFFECTS:  asserts if any of the preconditions of the constructor do not hold true, and
		//   does nothing otherwise; this function has no effect when not built in debug mode
		void assertConstruction() const;
};

#endif