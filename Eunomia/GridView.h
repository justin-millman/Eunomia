#ifndef EUNOMIA_GRID_VIEW
#define EUNOMIA_GRID_VIEW

#include <string>												// for string
#include <vector>												// for vector
#include "Utility.h"											// for VarStatus_e (because it's a template parameter)
#include "View.h"												// for View

/*
	A GridView is a type of View that displays data to the user in the form of a logic puzzle grid. A
	logic puzzle grid consists of "large squares" correspond to the intersection of two categories and
	"small squares" corresponding to the intersection of two items. The "large" and "small" squares are
	clearly demarcated, and both axes are labeled with the items (but not the category names). Items are
	listed in the order in which they are reported.

	Each cell represents the status of two items, i.e. whether or not they belong to the same group. If
	two items belong to the same group, the corresponding cell will contain a plus symbol ('+'). If the
	two items do not belong to the same group, the corresponding cell will contain a cross ('×'). However,
	it is possible that not enough information is known to determine whether or not two items belong to
	the same group; in such cases, the corresponding cell will simply be empty.

	Because a GridView must display both the items' strings and their intersection statuses, GridViews
	subscribe to all reported information and ignore nothing.
*/

class GridView : public View {
	public:
		// [Constructor]
		// REQUIRES: both <categories> and <itemsPerCategory> are at least 2
		GridView(const std::string& name, const std::string& puzzleName,
			size_t categories, size_t itemsPerCategory);

		// [Printer]
		// MODIFIES: standard output
		// EFFECTS:  prints the reported items and their intersection statuses to the console
		//   as a logic puzzle grid, where each large square of the grid corresponds to the
		//   intersection of two categories and each small square of the grid corresponds to
		//   the intersection of two items, with an '×' indicating that the two items of that
		//   cell are not in the same group and a '+' indicating that they do
		void display() const override;

	protected:
		// [Reporting Helpers]
		// REQUIRES: <varIndex> and <category> are non-negative, <varIndex> is valid based on
		//   the items that have been already reported
		// MODIFIES: <this>
		// EFFECTS:  remembers the information reported (either a new <status> or an <item>)
		//   to be printed later
		void recordStatus(int varIndex, VarStatus_e status) override;
		void recordItem(int category, int item, const std::string& itemStr) override;

	private:
		std::vector<std::vector<std::string>> items;
		std::vector<VarStatus_e> statuses;
};

#endif