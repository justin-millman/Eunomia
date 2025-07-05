#include "Expression.h"											// for Expression
#include "Model.h"												// for Model
#include "Parser.h"												// for Parser
#include "Puzzle.h"												// for Puzzle
#include "Utility.h"											// for VarStatus_e, EunomiaException
#include "View.h"												// for View
#include <algorithm>											// for find_if, remove_if
#include <memory>												// for unique_ptr
#include <string>												// for string
#include <utility>												// for move

using std::string;
using std::unique_ptr;
using std::find_if; using std::remove_if;
using std::move;

const char* const kDuplicatePuzzleMsg = "Error: A puzzle with that name already exists";
const char* const kNonexistPuzzleMsg = "Error: No puzzle with that name is currently open";
const char* const kDuplicateViewMsg = "Error: A view with that name already exists";
const char* const kNonexistViewMsg = "Error: No view with that name is currently open";

/*
	ViewHasName is a customizable functor whose overloaded function-call operator
	returns whether or not a View object (actually a uniqe_ptr to a View object)
	has a particular name.
*/
struct ViewHasName {
	public:
		// [Constructor]
		ViewHasName(const string& targetName);

		// [Functor]
		// REQUIRES: <view> is not a null-pointer
		// EFFECTS:  returns TRUE if <view>'s name is <target> and FALSE otherwise
		bool operator()(const unique_ptr<View>& view) const;

	private:
		string target;
};


// constructor
Model::Model() {}

// return function-static Singleton instace
Model& Model::getInstance() {
	static Model model{};
	return model;
}

// return number of active Puzzles
size_t Model::puzzleCount() const {
	return puzzles.size();
}

// return number of active Views
size_t Model::viewCount() const {
	return views.size();
}

// TRUE iff <name> used by active Puzzle
bool Model::nameIsUsed(const std::string& name) const {
	return (puzzles.find(name) != puzzles.cend());
}

// make sure that <puzzle> has a unique name, then add the <puzzle>/<parser>
// pair to the Puzzle map indexed on <puzzle>'s name
void Model::openPuzzle(const Puzzle& puzzle, const Parser& parser) {
	auto found = puzzles.find(puzzle.getName());
	if (found != puzzles.end()) {
		throw EunomiaException{ kDuplicatePuzzleMsg };
	}
	puzzles.emplace(puzzle.getName(), PuzzleInfo{ puzzle, parser });
}

// make sure that there's a Puzzle with the name <puzzleName>, then remove
// the <puzzle>/<parser> pair from the Puzzle map and any Views connected to
// that Puzzle from the Views list
void Model::closePuzzle(const string& puzzleName) {
	auto found = puzzles.find(puzzleName);
	if (found == puzzles.cend()) {
		throw EunomiaException{ kNonexistPuzzleMsg };
	}
	detachPuzzleViews(puzzleName);							// detach first so no error after Puzzle closed
	puzzles.erase(found);	
}

// constructor
ViewHasName::ViewHasName(const string& targetName)
	: target{ targetName } {}

// true if and only if <view>'s name is <target>
bool ViewHasName::operator()(const unique_ptr<View>& view) const {
	return (view->getName() == target);
}

// make sure that <view> has a unique name, then add it to the Views list
void Model::attachView(std::unique_ptr<View>&& view) {
	auto found = find_if(views.cbegin(), views.cend(), ViewHasName{ view->getName() });
	if (found != views.cend()) {
		throw EunomiaException{ kDuplicateViewMsg };
	}
	views.push_back(move(view));
}

// make sure that there's a View with the name <viewName>, then remove it
// from the Views list
void Model::detachView(const string& viewName) {
	auto found = find_if(views.cbegin(), views.cend(), ViewHasName{ viewName });
	if (found == views.cend()) {
		throw EunomiaException{ kNonexistViewMsg };
	}
	views.erase(found);
}

// make sure that there's a Puzzle with the name <puzzleName>, then remove all
// Views connected to that Puzzle from the Views list
void Model::detachPuzzleViews(const string& puzzleName) {
	if (puzzles.find(puzzleName) == puzzles.cend()) {
		throw EunomiaException{ kNonexistPuzzleMsg };
	}

	auto end = puzzles.cend();
	for (auto iter = puzzles.begin(); iter != end; ) {
		if (iter->second.puzzle.getName() == puzzleName) {
			puzzles.erase(iter++);
		}
		else {
			++iter;
		}
	}
}

// forward data to all Views
void Model::reportStatus(const string& puzzleName, int varIndex, VarStatus_e status) {
	for (const auto& view : views) {
		view->reportStatus(puzzleName, varIndex, status);
	}
}

// forward data to all Views
void Model::reportItem(const string& puzzleName, int category, int item, const string& itemStr) {
	for (const auto& view : views) {
		view->reportItem(puzzleName, category, item, itemStr);
	}
}

// make sure there's a Puzzle with the name <puzzleName>, then parse <clue> and apply
// it to the right Puzzle
bool Model::assess(const string& puzzleName, const string& clue) {
	auto found = puzzles.find(puzzleName);
	if (found == puzzles.cend()) {
		throw EunomiaException{ kNonexistPuzzleMsg };
	}

	auto parsedClue = found->second.parser.parse(clue);
	return found->second.puzzle.applyClue(*parsedClue);
}

// print all Views to the standard output stream
void Model::displayAll() const {
	for (const auto& view : views) {
		view->display();
	}
}

// make sure that there's a View with the name <viewName>, then print it to the standard
// output stream
void Model::displayView(const string& viewName) const {
	auto view = find_if(views.cbegin(), views.cend(), ViewHasName{ viewName });

	if (view == views.cend()) {
		throw EunomiaException{ kNonexistViewMsg };
	}
	(*view)->display();
}

// make sure that there's a Puzzle with the name <puzzleName>, then print all the Views
// connected to that Puzzle to the standard output stream
void Model::displayPuzzleViews(const string& puzzleName) const {
	if (puzzles.find(puzzleName) == puzzles.cend()) {
		throw EunomiaException{ kNonexistPuzzleMsg };
	}

	for (const auto& view : views) {
		if (view->getPuzzle() == puzzleName) {
			view->display();
		}
	}
}