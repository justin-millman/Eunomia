#ifndef EUNOMIA_CONTROLLER
#define EUNOMIA_CONTROLLER

#include <memory>												// for unique_ptr
#include "Parser.h"												// because Parser is a template parameter
#include "Puzzle.h"												// because Puzzle is a template parameter

/*

*/

class Controller {
	public:
		// [Constructors]
        // REQUIRES: <fileName> is a valid text file name with properly-formatted content
		// MODIFIES: standard input, standard output
		// EFFECTS:  prompts the user for metadata concerning the logic puzzle and then reads
		//   from standard input to construct the puzzle
		Controller();
        Controller(const char* fileName);

		// [Runners]
		// MODIFIES: standard input, standard output, <this>
		// EFFECTS:  reads commands from standard input and executes them against the simulation
		//   environment until the user issues a "quit" command, ending the simulation
		void run();

	private:
		std::unique_ptr<Puzzle> puzzle;
		std::unique_ptr<Parser> parser;

        //
        void init();

		// MODIFIES: standard input, standard output, <this>
		// EFFECTS:  reads a single command from standard input and performs the corresponding
		//   action; returns TRUE if and only if the user does not enter the 'quit' command
		bool readCommand();

		// MODIFIES: standard input, standard output, <this>
		// EFFECTS:  reads a line from standard input, parses the input, and applies the resulting
		//   clue to <puzzle>
		void evaluateClue();

		// MODIFIES: standard output
		// EFFECTS:  prints <grid> to standard output
		void printGrid() const;

		// MODIFIES: standard output
		// EFFECTS:  prints <summary> to standard output
		void printSummary() const;
};

#endif