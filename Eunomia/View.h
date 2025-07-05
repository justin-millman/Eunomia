#ifndef EUNOMIA_VIEW
#define EUNOMIA_VIEW

#include <string>												// for string

enum class VarStatus_e;

/*
	A View is an abstract interface for displaying data to the user via the console in a specific way.
	Each View has a name and is tied to a specific Puzzle name; these specifics are set at construction
	and can't be changed afterward. The name is used to identify the View, while the name of the Puzzle
	is used to selectively attend to information that is reported.

	Two types of information can be reported to a View, and View can choose how -- or if at all -- to
	handle each type. The first is a variable status, and the second is an item's item-string. By
	default, all View's ignore both pieces of information, but derived classes can (and should) implement
	custom handlers for remembering them. (The handlers are conveniently provided as virtual functions).

	When a View displays, it prints the information that it has remembered to the standard output
	stream in a way defined by the specific derived class.
*/

class View {
	public:
		// [Constructor]
		View(const std::string& name, const std::string& puzzleName);

		// [Destructor]
		virtual ~View();

		// [Description Accessors]
		// EFFECTS:  returns the name of <this> or the name of the Puzzle that <this> is
		//   meant to display
		const std::string& getName() const;
		const std::string& getPuzzle() const;

		// [Reporting Interface]
		// REQUIRES: <varIndex>, <category>, and <item> are non-negative, <varIndex> is valid
		//   based on the items that have been already reported
		// MODIFIES: <this>
		// EFFECTS:  if <puzzle> is the same as the <puzzleName> with which <this> was
		//   constructed and the derived class so desires, the reported information
		//   (a new <status> or an <item>) is remembered so that it can be printed later;
		//   if <puzzle> does not match or the derived class does not wish, the information
		//   is ignored
		void reportStatus(const std::string& puzzle, int varIndex, VarStatus_e status);
		void reportItem(const std::string& puzzle, int category, int item, const std::string& itemStr);

		// [Printer]
		// MODIFIES: standard output
		// EFFECTS:  prints <this> to the standard output stream based on the manner of the
		//   derived class and the information that was reported previously and remembered
		virtual void display() const = 0;

	protected:
		// [Reporting Helpers]
		// REQUIRES: <varIndex> and <category> are non-negative,  <varIndex> is valid based on
		//   the items that have been already reported
		// MODIFIES: <this>
		// EFFECTS:  if the derived class wishes, the reported information (a new <status> or
		//   an <item>) is remembered; by default, this information is ignored
		virtual void recordStatus(int varIndex, VarStatus_e status);
		virtual void recordItem(int category, int item, const std::string& itemStr);

	private:
		std::string name;
		std::string puzzleName;
};

#endif