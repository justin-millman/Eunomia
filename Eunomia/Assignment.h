#ifndef EUNOMIA_ASSIGNMENT
#define EUNOMIA_ASSIGNMENT

#include <vector>												// for vector

/*
	An Assignment is a set of Boolean values (TRUE or FALSE) that correspond to a number of variables,
	that number being set at construction and thereafter immutable. An accessor function exists to
	determine the number of variables covered by a particular Assignment, and those variables are
	always indexed from [0, size()].

	The currently-assigned value of any variable can be accessed using its zero-based index. The
	assigned values can also be changed an unlimited number of times via the truth setters, which
	also require the variable's zero-based index.

	Assignments provide TRUE- and FALSE- specific iterators that iterate over each variable in the
	Assignment that currently has a specific assigned value. It is not possible to modify the value
	of any variable directly through an iterator, and modifying the value indirectly or through the
	Assignment itself renders the iterators invalid.
*/

class Assignment {
	public:
		// [Constructor]
		// REQUIRES: <varCount> is positive and nonzero
		explicit Assignment(size_t varCount);

		// [Size Accessor]
		// EFFECTS:  returns the number of variables for which <this> holds an assigned
		//   truth value
		size_t size() const;

		// [Truth Accessor]
		// REQUIRES: <varIndex> is non-negative and less than <size()>
		// EFFECTS:  returns the assigned truth value for the variable at index <varIndex>
		bool getAssignedValue(int varIndex) const;

		// [Truth Setters]
		// REQUIRES: <varIndex> is non-negative and less than <size()>
		// MODIFIES: invalidates any active iterators
		// EFFECTS:  sets the assigned truth value for the variable at index <varIndex>
		//   to be either TRUE or FALSE
		void setTrue(int varIndex);
		void setFalse(int varIndex);

		/*
			A VarIterator is a type of forward iterator that is attached to a particular
			Assignment object and only stops at elements with a particular assigned
			value, either TRUE or FALSE. Dereferencing a VarIterator gives the zero-based
			index of the element currently "pointed" at.
		*/
		class VarIterator {
			public:
				// [Increment Operators]
				// MODIFIES: <this>
				// EFFECTS:  pre- or post-increments <this> to the next element of
				//   <assignment> whose assigned value in <assignment> is <target>
				VarIterator& operator++();
				VarIterator operator++(int);

				// [Dereference Operator]
				// EFFECTS:  returns the index of the variable currently "pointed" to by
				//   <this>, whose assigned value in <assignment> must be <target>; throws
				//   and out_of_range exception if <this> "points" past the end of the
				//   Assignment it is iterating over
				int operator*() const;

				// [Equality Operators]
				// EFFECTS:  returns TRUE if both <this> and <rhs> are "pointing" to the
				//   same element of the same Assignment with the same target, and returns
				//   FALSE otherwise; returns the opposite for the inquality operator
				bool operator==(const VarIterator& rhs) const;
				bool operator!=(const VarIterator& rhs) const;

				friend Assignment;

			private:
				int position;
				const Assignment* assignment;
				bool target;

				// [Constructor]
				// REQUIRES: <assign> is not a null pointer,
				//   <initialPos> is non-negative and less than or equal to <assign.size()>
				//   (where equal to <assign.size()> indicates "off the end")
				VarIterator(const Assignment* assign, bool target, int initialPos);
		};
		using iterator_t = VarIterator;

		// [Iterators]
		// EFFECTS:  for the "begin" iterators, returns an iterator to the first element of
		//   <this> whose assigned value is either TRUE or FALSE, or "one past the last"
		//   element of <this> if no such element exists; for the "end" iterators, returns
		//   an iterator to "one past the last" element of <this>
		iterator_t trueBegin() const;
		iterator_t trueEnd() const;
		iterator_t falseBegin() const;
		iterator_t falseEnd() const;

	private:
		std::vector<bool> truths;

		// [Iterator Generator Helpers]
		// EFFECTS:  creates either a "begin" iterator that iterates over variables with an
		//   assigned value of <target> or an "end" iterator that is "one past the last"
		//   variable with an assigned value of <target>
		iterator_t makeBeginIterator(bool target) const;
		iterator_t makeEndIterator(bool target) const;
};

#endif