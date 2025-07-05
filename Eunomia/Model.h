#ifndef EUNOMIA_MODEL
#define EUNOMIA_MODEL

#include <memory>												// for unique_ptr (because it's a template parameter)
#include <string>												// for string
#include <vector>												// for vector
#include <unordered_map>										// for unordered_map
#include "Parser.h"												// for Parser (because it's a using'd member variable)
#include "Puzzle.h"												// for Puzzle (because it's a using'd member variable)

enum class VarStatus_e;
class View;

/*
	The Model is a Singleton class that maintains the current state of the user's data: Puzzles and Views. Both
	Puzzles and Views can be added ("opened" or "attached," respectively) or removed ("closed" or "detached,"
	respectively) to/from the Model, which keeps track of the "active" Puzzles and Views for interaction. Puzzles
	and Views are identified by their name, which must be unique among all active Puzzles or Views. It is possible
	to access the number of Puzzles or number of Views currently maintained by the Model, and is is also possible
	to determine whether or not a particular puzzle name is in use.

	When a Puzzle is opened on the Model, it must be opened with a designated Parser that is used to parse clues
	for that Puzzle. This Puzzle/Parser pair persists for the lifetime of the Puzzle; once the Puzzle is opened,
	the Parser cannot be modified.

	Each View that is attached to the Model is connected to a specific Puzzle; that is the Puzzle whose data it
	displays to the user. Views can be removed individually by name or in batches by the connected Puzzle. When a
	Puzzle is closed, all the Views connected to that Puzzle are detached immediately. When a new View is attached,
	all active Puzzles are asked to broadcast their current state to the Model in order to get the new View up to
	date.

	Data is reported to the Model by Puzzles, and the data is forwarded to all of the active Views, which may handle
	the information in some specific fashion. The Model does not keep track of whether or not data has changed since
	the last time it was reported.

	Clues are applied to Puzzles through the Model, which will first parse the clue using the appopriate Parser.
*/

class Model {
	public:
		// [Singleton Accessor]
		// EFFECTS:  returns a reference to the Singelton instance of the Model class
		static Model& getInstance();

		// [Size Accessors]
		// EFFECTS:  returns the number of active Puzzles or the number of active Views
		size_t puzzleCount() const;
		size_t viewCount() const;

		// [Name Verifier]
		// EFFECTS:  returns TRUE if <name> is the name of an active Puzzle and returns FALSE
		//   otherwise
		bool nameIsUsed(const std::string& name) const;

		// [Puzzle Managers]
		// MODIFIES: list of Puzzles, list of Parsers, list of Views
		// EFFECTS:  adds the given <puzzle>/<parser> combination to the list of active Puzzles,
		//   or removes the Puzzle with the name <puzzleName> from the list of active Puzzles;
		//   if Puzzle in the active list with the same name as <puzzle> exists or if no active
		//   Puzzle has the name <puzzleName>, throws a EunomiaException; if a Puzzle is closed,
		//   all active Views related to that Puzzle are detached
		void openPuzzle(const Puzzle& puzzle, const Parser& parser);
		void closePuzzle(const std::string& puzzleName);

		// [View Managers]
		// MODIFIES: list of Views
		// EFFECTS:  adds <view> to the list of active Views, or removes from the list of active
		//   Views either the view with the name <viewName> or all active Views whose associated
		//   Puzzle has the name <puzzleName>; if a View in the active list with the same name as
		//   <view> exists, or if no active View has the name <viewName>, or if no active Puzzle
		//   has the name <puzzleName>, throws a EunomiaException; when a new View is added,
		//   each Puzzle is told to broadcast its state to get the new View up to date
		void attachView(std::unique_ptr<View>&& view);
		void detachView(const std::string& viewName);
		void detachPuzzleViews(const std::string& puzzleName);

		// [Reporters]
		// MODIFIES: active Views
		// EFFECTS:  forwards the reported information to each of the active Views, which may or
		//   may not further handle the information in some way
		void reportStatus(const std::string& puzzleName, int varIndex, VarStatus_e status);
		void reportItem(const std::string& puzzleName, int category, int item, const std::string& itemStr);

		// [Clue Assessor]
		// MODIFIES: the active Puzzle with the name <puzzleName>
		// EFFECTS:  uses the correct Parser to parse <clue> into a Boolean Expression and then
		//   applies that Expression to the Puzzle with name <puzzleName>; if <clue> cannot be
		//   properly parsed (becaues it is malformed) or if no active Puzzle exists with the
		//   name <puzzleName>, throws a EunomiaException
		bool assess(const std::string& puzzleName, const std::string& clue);

		// [Displayers]
		// MODIFIES: standard output
		// EFFECTS:  prints all the active Views, the active View with the name <viewName>, or
		//   all active Views whose associated Puzzle has the name <puzzleName> to the standard
		//   output stream, each separated by a newline character; if no active View has the name
		//   <viewName> or no active Puzzle has the name <puzzleName>, throws a EunomiaException
		void displayAll() const;
		void displayView(const std::string& viewName) const;
		void displayPuzzleViews(const std::string& puzzleName) const;

	private:
		using PuzzleInfo = struct { Puzzle puzzle; Parser parser; };

		std::unordered_map<std::string, PuzzleInfo> puzzles;
		std::vector<std::unique_ptr<View>> views;

		// [Constructor]
		Model();
};

#endif