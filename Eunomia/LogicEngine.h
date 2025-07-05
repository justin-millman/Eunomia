#ifndef EUNOMIA_LOGIC_ENGINE
#define EUNOMIA_LOGIC_ENGINE

#include <vector>												// for vector
#include "Assignment.h"											// for Assignment (because it's a template parameter)

class Expression;
enum class VarStatus_e;

/*
	A LogicEngine is an evaluative computer that tracks the evaluation of a logic Puzzle against one or
	more clues. The logic Puzzle that the LogicEngine is responsible for handling is set at construction
	and cannot thereafter be changed. The number of variables that the LogicEngine treats is equal to the
	total number of inter-category combinations of items in that logic Puzzle.

	At construction, a LogicEngine generates a finite set of possible Boolean Assignments of all the
	variables for which it is responsible. These Assignments correspond to the sum total of all possible
	solutions to the logic Puzzle. Notably, because of the rules of logic Puzzles, a logic Puzzle with
	~N~ variables has significantly fewer than ~2^N~ possible solutions.

	The primary action that a LogicEngine can perform is to evaluate a clue. A clue is a Boolean Expression
	consisting of variables that the LogicEngine is responsible for handling. When a LogicEngine evaluates
	a clue, it considers all active Assignments and eliminates those that do not cause the clue to evaluate
	to TRUE. These Assignments are irrecoverably discarded and never again considered. It is not possible to
	undo an evaluation of a clue. However, if a clue is evaluated that eliminates all possible Assignments,
	and is thus an inconsistent clue, the eliminations will be rolled back and the LogicEngine will be in a
	state as if the clue were never evaluated.

	As the LogicEngine eliminates possible Assignments, it keeps track of the status of each variable it is
	responsible for. The status of a variable is either "guaranteed to be true" (meaning that the variable
	has an assigned value of TRUE in all remaining viable Assignments), "guaranteed to be false" (meaning
	that the variable has an assigned value of FALSE in all remaining viable assignments), or "unknown"
	(meaning that the variable has an assigned value of TRUE in at least one of the remaining viable
	Assignments and an assigned value of FALSE in at least one other). The status of a variable can be
	gleaned via an index-based accessor. Additionally, the status of the puzzle ("solved," meaning that every
	variable has a non-unknown status, or "unsolved") can be determined through an inquiry.
*/

class LogicEngine {
	public:
		// [Constructor]
		// REQUIRES: <numCategories> and <itemsPerCategory> are both at least 2
		LogicEngine(size_t numCategories, size_t itemsPerCategory);

		// [Status Accessor]
		// REQUIRES: <varIndex> is non-negative and less than the total number of variables
		//   implied by the <puzzle> used to construct <this>
		// EFFECTS:  returns the status of the <varIndex>th variable based on the logic
		//   peformed thus far
		VarStatus_e getStatus(int varIndex) const;

		// [Solved Checker]
		// EFFECTS:  returns TRUE if the status of every variable is either "guaranteed to
		//   be true" or "guaranteed to be false," and returns FALSE otherwise
		bool solved() const;

		// [Clue Evaluator]
		// REQUIRES: each variable that forms part of <clue> is within the scope of <this>,
		//   meaning that each variable's ID is less than the total number of variables implied
		//   by the <puzzle> used to construct <this>
		// MODIFIES: <this>
		// EFFECTS:  eliminates possible Assignments that don't result in a TRUE evaluation
		//   for <clue> and updates the statuses of variables depending on the Assignments
		//   eliminated; if <clue> was an inconsistent clue (meaning that it somehow contradcits
		//   at least one clue previously evaluated by <this>), returns FALSE and makes no
		//   changes whatsoever; returns TRUE otherwise
		bool evaluateClue(const Expression& clue);

	private:
		std::vector<Assignment> assignments;
		std::vector<long long> variables;
};

#endif