#ifndef EUNOMIA_SUMMARY_VIEW
#define EUNOMIA_SUMMARY_VIEW

#include <unordered_map>										// for unordered_map
#include <string>												// for string
#include <vector>												// for vector
#include "View.h"												// for View

enum class VarStatus_e;

/*
	A SummaryView is a type of View that displays data to the user as a table of groups, with each
	group occupying a single row and each column representing a different category. Groups are formed
	as items' intersection statuses are reported as "true"; once such as status is thusly reported, it
	cannot be undone. In other words: it is not possible to break up a group once it has been formed.

	Groups are formed transitively as items' intersections are marked as "true." For example, if the
	status of the "ItemA/ItemB" interseciton is marked as "true," then those two items are said to
	be in the same group. If subsequently the intersection of "ItemA/ItemC" is marked as true, then
	ItemC joins the "ItemA + ItemB" group. If ItemC had previously been in a group with ItemD, then
	ItemD would join the group as well, resulting in a group of four items: ItemA, ItemB, ItemC, and
	ItemD. If an item is not in any group, then it will not be listed at all in the display.

	Because of the way in which groups are formed, it is not possible to "unform" a group. That means
	that once the intersection of two items is marked as "true," it cannot be unmarked (i.e. marked as
	"false" or "unknown"). However, because SummaryViews are only concerned with what is a group and
	not what isn't, an intersection of items marked as "false" or "unknown" can later be marked as
	"true" without any issue.

	When displaying, SummaryViews assume that no two items of the same group are from the same category.
	If this is the case (because of user error), which item is displayed is not concretely defined.
*/

class SummaryView : public View {
	public:
		// [Constructor]
		// REQUIRES: both <categories> and <itemsPerCategory> are at least 2
		SummaryView(const std::string& name, const std::string& puzzleName,
			size_t categories, size_t itemsPerCategory);

		// [Printer]
		// MODIFIES: standard output
		// EFFECTS:  prints the reported items as groups based on their reported intersection
		//   statuses, with each group its own row and each category its own column, with the
		//   assumption that no group has more than one item from a given category; only groups
		//   with at least two items are printed
		void display() const override;

	protected:
		// [Reporting Helpers]
		// REQUIRES: <varIndex> and <category> are non-negative, <varIndex> is valid based on
		//   the items that have been already reported, the variable with <varIndex> has not
		//   already had its status updated, updating the status of <varIndex> does not cause
		//   a group to have more than one item of a single category
		// MODIFIES: <this>
		// EFFECTS:  remembers the information reported (either a new <status> or an <item>)
		//   to be printed later
		void recordStatus(int varIndex, VarStatus_e status) override;
		void recordItem(int category, int item, const std::string& itemStr) override;

	private:
		using Node_t = struct { int reprIdx; int rank; };
		using Var_t = struct { int item1Cat; int item1ItemIdx; int item2Cat; int item2ItemIdx; };

		std::unordered_map<int, Var_t> varMap;					// maps variable indices to the two items that variable represents
		std::vector<std::vector<std::string>> items;
		mutable std::vector<Node_t> disjointSet;				// mutable because might get updated by findRepr() via "path compression"
		size_t itemsPerCategory;

		// REQUIRES: <varMap> is empty, both <categories> and <itemsPerCategory> are at least 2
		// MODIFIES: <varMap>
		// EFFECTS:  loads the elements of <varMap> with each possible variable and the two items
		//   whose intersection that variable represents
		void loadVarMap(size_t categories, size_t itemsPerCategory);

		// REQUIRES: <nodeIdx> is non-negative and less than <disjointSet.size()>
		// MODIFIES: elements of <disjointSet>
		// EFFECTS:  returns the group representative of the Node in <disjointSet> with index
		//   <nodeIdx>, performing path compression in the process
		int findRepr(int nodeIdx) const;

		// REQUIRES: <node1Idx> and <node2Idx> are both non-negative and less than
		//   <disjointSet.size()>
		// MODIFIES: elements of <disjointSet>
		// EFFECTS:  updates the representative indices of <node1Idx> and <node2Idx>, as well
		//   as all the Nodes in their groups, to be the same, deciding on the new representative
		//   by weighing the ranks
		void joinNodes(int node1Idx, int node2Idx);
};

#endif