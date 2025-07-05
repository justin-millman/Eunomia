#include "Category.h"
#include "Controller.h"
#include "EunomiaStream.h"
#include "Expression.h"
#include "GridView.h"
#include "Model.h"
#include "Parser.h"
#include "Puzzle.h"
#include "SummaryView.h"
#include "Utility.h"
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

using std::unique_ptr; using std::invalid_argument;
using std::cin; using std::getline; using std::ifstream;
using std::vector; using std::string;
using std::move;
using std::endl;

// MODIFIES: standard input, standard output
// EFFECTS:  prompts the user for information about the puzzle and reads the
//   content necessary to construct the puzzle
unique_ptr<Puzzle> buildPuzzle();

// MODIFIES: standard input, standard output
// EFFECTS:  prompts the user for a parse string for each of the variables of <puzzle>
//   and reads the strings to construct the parser for <puzzle>
unique_ptr<Parser> buildParser(const Puzzle& puzzle);

// REQUIRES: <catIdx> >= 1, <itemsPer> >= 1
// MODIFIES: standard input, standard output
// EFFECTS:  prompts the user for a name and <itemsPer> items to build a new Category
Category readCategory(int catIdx, int itemsPer);


// prompt user for puzzle info and build puzzle
Controller::Controller()
    : puzzle{ nullptr }, parser{ nullptr } {
	
    puzzle = buildPuzzle();
	eout << "\n";
	parser = buildParser(*puzzle);

    init();
}

// read puzzle info from the designated file
Controller::Controller(const char* fileName) 
    : puzzle{ nullptr }, parser{ nullptr } {

    ifstream is{ fileName };
    if (!is.is_open()) {
         throw invalid_argument{ "Could not open file '" + string{ fileName } + "'" };
    }

    eout << "Reading puzzle and parser information from '" << fileName << "' . . ." << endl;

    string puzzleName = readLine(is);
    string puzzleDesc = readLine(is);

    int catCount = readInt(is);
    int itemPerCat = readInt(is);
    eout << "\t" << "Building puzzle with " << catCount << " categories of " << itemPerCat << " items each . . ." << endl;

    vector<Category> cats{};
    vector<string> items{};
    cats.reserve(catCount);
    items.reserve(itemPerCat);
    string catName;
    string itemName;
    for (int c = 0; c < catCount; ++c) {
        items.clear();
        catName = readLine(is);
        eout << "\t\t" << "Category '" << catName << "':  ";
        for (int i = 0; i < itemPerCat; ++i) {
            itemName = readLine(is);
            eout << (i > 0 ? ", " : "") << itemName;
            items.push_back(itemName);
        }
        eout << endl;
        cats.push_back(Category{ catName, items.begin(), items.end() });
    }
    puzzle = unique_ptr<Puzzle>{ new Puzzle{ puzzleName, puzzleDesc, cats.cbegin(), cats.cend() } };

    int vars = variablesNeeded(catCount, itemPerCat);
    eout << "\t" << "Building parser for " << vars << " variables . . ." << endl;

    vector<string> parseStrings{};
    parseStrings.reserve(vars);
    for (int i = 0; i < vars; ++i) {
        parseStrings.push_back(readString(is));
    }
    parser = unique_ptr<Parser>{ new Parser{ parseStrings.cbegin(), parseStrings.cend() } };

    eout << "Puzzle and praser completed!";

    init();
}

//
void Controller::init() {
    Model::getInstance().openPuzzle(*puzzle, *parser);

    auto grid = unique_ptr<GridView>{ new GridView{ "Grid", puzzle->getName(), puzzle->numCategories(), puzzle->itemsPerCategory() } };
    auto summary = unique_ptr<SummaryView>{ new SummaryView{ "Summary", puzzle->getName(), puzzle->numCategories(), puzzle->itemsPerCategory() } };

    Model::getInstance().attachView(move(grid));
    Model::getInstance().attachView(move(summary));
    puzzle->broadcastState();
}

// run the Controller
void Controller::run() {
	eout << "\n\n\nWelcome to Eunomia!!\n" << string(35, '~') << "\n";

	bool go = true;
	while (go) {
		try {
			eout << "\nEunomia >  ";
			go = readCommand();
		}
		catch (const EunomiaException& ex) {
			while (cin.get() != '\n');
			eout << "Error: " << ex.what();
		}
		catch (...) {
			eout << "Runtime error . . . aborting . . .";
			return;
		}
	}
}

// build puzzle
unique_ptr<Puzzle> buildPuzzle() {
    eout << "Name of Puzzle:  ";
    string name = readLine(cin);
	eout << "Description of Puzzle:  ";
    string desc = readLine(cin);

	int numCategories;
	int itemsPer;
	eout << "\n# of Categories:  ";
	numCategories = readInt(cin);
	eout << "# of Items per Category:  ";
	itemsPer = readInt(cin);

	eout << "\n";
	vector<Category> categories;
	categories.reserve(numCategories);
	for (int c = 1; c <= numCategories; ++c) {
		categories.push_back(readCategory(c, itemsPer));
	}

	return unique_ptr<Puzzle>{ new Puzzle{ name, desc, categories.cbegin(), categories.cend() }};
}

// build category
Category readCategory(int catIdx, int itemsPer) {
	string name;
	vector<string> items;
	items.reserve(itemsPer);

	eout << "Name of Category #" << catIdx << ":  ";
	name = readLine(cin);
	for (int i = 1; i <= itemsPer; ++i) {
		eout << "\t Item " << catIdx << "." << i << ":  ";
		items.push_back(readString(cin));
	}

	return Category{ name, items.cbegin(), items.cend() };
}

// build parser
unique_ptr<Parser> buildParser(const Puzzle& puzzle) {
	auto categories = puzzle.numCategories();
	auto itemsPer = puzzle.itemsPerCategory();

	vector<string> parseStrings{};
	parseStrings.reserve(puzzle.numCategories());
	for (size_t catIdx = categories; catIdx > 1; --catIdx) {							// for each category (except for Category #1)
		size_t cat = catIdx % categories;												//   to get the first category to be category 0, then reverse
		for (size_t item = 0; item < itemsPer; ++item) {								//   for each item in the category
			const string& itemStr = puzzle[cat][item];
			for (size_t otherCat = 1; otherCat < catIdx; ++otherCat) {					//     for each paired category along the y-axis
				for (size_t otherItem = 0; otherItem < itemsPer; ++otherItem) {			//       for each item in the other category
					eout << "Parse String for (" << itemStr << ") ++ "
						<< "(" << puzzle[otherCat][otherItem] << ") :  ";
					parseStrings.push_back(readString(cin));
				}
			}
		}
	}

	return unique_ptr<Parser>{ new Parser{ parseStrings.cbegin(), parseStrings.cend() }};
}

// read a command and execute the appropriate function
bool Controller::readCommand() {
	string cmd = readString(cin);
	if (cmd.length() > 1) {
		throw EunomiaException{ "Invalid command" };
	}

	switch (cmd[0]) {
		case 'e':
		case 'E':
			evaluateClue();
			return true;
		case 'g':
		case 'G':
			printGrid();
			return true;
		case 'p':
		case 'P':
			printGrid();
			printSummary();
			return true;
		case 'q':
		case 'Q':
			return false;
		case 's':
		case 'S':
			printSummary();
			return true;
		default:
			throw EunomiaException{ "Invalid command" };
	}
}

// read a clue and evaluate it
void Controller::evaluateClue() {
	string clue;

    getline(cin, clue);                 // clear stream
    eout << "Enter Clue:  ";
	getline(cin, clue);

    if (!Model::getInstance().assess(puzzle->getName(), clue)) {
        eout << "This clue is logically inconsistent with previous clues" << endl;
    }
}

// print the grid view
void Controller::printGrid() const {
	Model::getInstance().displayView("Grid");
}

// print the summary view
void Controller::printSummary() const {
	Model::getInstance().displayView("Summary");
}