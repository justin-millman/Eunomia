#include "stdafx.h"												// for UnitTesting framework
#include "CppUnitTest.h"										// for UnitTesting framework
#include "../Eunomia/Category.h"								// for Category
#include "../Eunomia/Clause.h"									// for Clause
#include "../Eunomia/Expression.h"								// for Expression
#include "../Eunomia/Literal.h"									// for Literal
#include "../Eunomia/Puzzle.h"									// for Puzzle
#include "../Eunomia/Utility.h"									// for Operator_e
#include <memory>												// for unique_ptr, make_unique
#include <set>													// for set
#include <string>												// for string
#include <vector>												// for vector

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using std::string;
using std::vector; using std::set;
using std::unique_ptr; using std::make_unique;


static bool operator<(const Category& lhs, const Category& rhs) {
	return (lhs[0] < rhs[0]);
}

static unique_ptr<Expression> makeExpr(const Literal& literal, bool negate = false) {
	auto expr = make_unique<Literal>(literal);
	if (negate) {
		expr->negate();
	}
	return expr;
}

namespace Atropos {
	TEST_CLASS(PuzzleTester) {
		public:
			TEST_METHOD(Puzzle_ConstructVector) {
				vector<string> items1{ "Muenster", "Cheddar", "Mozzarella", "Asiago" };
				vector<string> items2{ "Zebra", "Horse", "Wild Ass", "Donkey" };
				vector<string> items3{ "Rifle", "Pistol", "Shotgun", "Handgun" };
				
				vector<Category> categories{
					Category{ "Cheeses", items1.cbegin(), items1.cend() },
					Category{ "Equids", items2.cbegin(), items2.cend() },
					Category{ "Firearms", items3.cbegin(), items3.cend() }
				};

				const string name = "Puzzle1";
				const string description = "A 3-category puzzle";
				Puzzle puzzle{ name, description, categories.cbegin(), categories.cend() };

				Assert::IsTrue(puzzle.getName() == name);
				Assert::IsTrue(puzzle.getDescription() == description);
				Assert::IsTrue(puzzle.numCategories() == categories.size());
				Assert::IsTrue(puzzle.itemsPerCategory() == items1.size());
				Assert::IsTrue(puzzle.numVariables() == 48);
				Assert::IsTrue(!puzzle.solved());
			}

			TEST_METHOD(Puzzle_ConstructArray) {
				vector<string> items1{ "Chanukkah", "Purim", "Yom Kippur", "Rosh HaShannah",
					"Tu B'Shvat" };
				vector<string> items2{ "Cholesterol", "Carbohydrate", "Carbon", "Corona",
					"Constellation" };
				vector<string> items3{ "Washington", "Roosevelt", "Nixon", "Obama", "Monroe" };

				Category categories[]{
					Category{ "Jewish Holidays", items1.cbegin(), items1.cend() },
					Category{ "C-Science", items2.cbegin(), items2.cend() },
					Category{ "Presidents", items3.cbegin(), items3.cend() }
				};

				size_t num = sizeof(categories) / sizeof(Category);
				const string name = "Random Puzzle #5";
				const string description = "A test puzzle for array-based construction";
				Puzzle puzzle{ name, description, categories, categories + num };

				Assert::IsTrue(puzzle.getName() == name);
				Assert::IsTrue(puzzle.getDescription() == description);
				Assert::IsTrue(puzzle.numCategories() == num);
				Assert::IsTrue(puzzle.itemsPerCategory() == items1.size());
				Assert::IsTrue(puzzle.numVariables() == 75);
				Assert::IsTrue(!puzzle.solved());
			}

			TEST_METHOD(Puzzle_ConstructSet) {
				vector<string> items1{ "Diet Pepsi", "Orange Crush", "Dr. Pepper",
					"A&W Root Beer", "Cream Soda" };
				vector<string> items2{ "1870s", "1230s", "1950s", "1760s", "540s" };
				vector<string> items3{ "Amethyst", "Garnet", "Sapphire", "Emerald",
					"Aquamarine" };
				vector<string> items4{ "Kraken", "Mermaid", "Hippocampus",
					"Ichthyocentaur", "Leviathan" };
				
				set<Category> categories{
					Category{ "Soft Drinks", items1.cbegin(), items1.cend() },
					Category{ "Decades", items2.cbegin(), items2.cend() },
					Category{ "Gemstones", items3.cbegin(), items3.cend() },
					Category{ "Mythical Sea Creatures", items4.cbegin(), items4.cend() }
				};

				const string name = "Whajamajiggit";
				const string description = "Sodas vs. Ten-Year Spans vs. Minerals vs. Sea Creatures";
				Puzzle puzzle{ name, description, categories.cbegin(), categories.cend() };

				Assert::IsTrue(puzzle.getName() == name);
				Assert::IsTrue(puzzle.getDescription() == description);
				Assert::IsTrue(puzzle.numCategories() == categories.size());
				Assert::IsTrue(puzzle.itemsPerCategory() == items1.size());
				Assert::IsTrue(puzzle.numVariables() == 150);
				Assert::IsTrue(!puzzle.solved());
			}
			
			TEST_METHOD(Puzzle_Index) {
				vector<string> items1{ "Addition", "Multiplication", "Factorial",
					"Integration", "Partial Differentiation", "Modular Division" };
				vector<string> items2{ "Tibia", "Radius", "Patella", "Coccyx",
					"Phalanges", "Metatarsals" };
				vector<string> items3{ "Buffy Summers", "Willow Rosenberg",
					"Rupert Giles", "Xander Harris", "Angel", "Spike" };
				
				vector<Category> categories{
					Category{ "Mathematical Operations", items1.cbegin(), items1.cend() },
					Category{ "Bones", items2.cbegin(), items2.cend() },
					Category{ "Buffy Characters", items3.cbegin(), items3.cend() }
				};
				Puzzle puzzle{ "Another Puzzle", "", categories.cbegin(), categories.cend() };

				size_t num = categories.size();
				size_t items = categories[0].size();
				for (size_t i = 0; i < num; ++i) {
					Assert::IsTrue(puzzle[i].getName() == categories[i].getName());
					for (size_t j = 0; j < items; ++j) {
						Assert::IsTrue(puzzle[i][j] == categories[i][j]);
					}
				}
			}

			TEST_METHOD(Puzzle_Iterate) {
				vector<string> items1{ "Vietnamese", "Cantonese", "Hindi" };
				vector<string> items2{ "Boston Terrier", "Greyhound", "Shih Tzu" };
				vector<string> items3{ "Hammer", "Socket Wrench", "Screwdriver" };
				vector<string> items4{ "Shaka Zulu", "Anwar Sadat", "Idi Amin" };

				vector<Category> categories{
					Category{ "Asian Languages", items1.cbegin(), items1.cend() },
					Category{ "Dog Breeds", items2.cbegin(), items2.cend() },
					Category{ "Tools", items3.cbegin(), items3.cend() },
					Category{ "Historical African Leaders", items4.cbegin(), items4.cend() }
				};
				Puzzle puzzle{ "Yet Another Puzzle", "?", categories.cbegin(), categories.cend() };

				size_t count = 0;
				size_t items = categories[0].size();
				for (const auto& cat : puzzle) {
					for (size_t i = 0; i < items; ++i) {
						Assert::IsTrue(cat[i] == categories[count][i]);
					}
					Assert::IsTrue(cat.getName() == categories[count++].getName());
				}
				Assert::IsTrue(count == categories.size());
				Assert::IsTrue(count == puzzle.numCategories());
			}

			TEST_METHOD(Puzzle_ApplyCluesUnsolved) {
				vector<string> items1{ "The Avatar and the Firelord", "The Storm",
					"Lake Laogai", "Sozin's Comet - Part III" };
				vector<string> items2{ "Wednesdsay", "Friday", "Sunday", "Tuesday" };

				vector<Category> categories{
					Category{ "ATLA Episodes", items1.cbegin(), items1.cend() },
					Category{ "Days of the Week", items2.cbegin(), items2.cend() }
				};
				Puzzle puzzle{ "Yet Again Another Puzzle", "meaningless description",
					categories.cbegin(), categories.cend() };

				size_t num = items1.size();
				vector<Literal> literals;
				literals.reserve(puzzle.numVariables());
				for (size_t i = 0; i < num; ++i) {
					for (size_t j = 0; j < num; ++j) {
						literals.emplace_back(i * num + j, items1[i] + "|" + items2[j]);
					}
				}

				auto c1 = make_unique<Clause>(makeExpr(literals[0]), makeExpr(literals[15]), Operator_e::Or);
				auto c2 = make_unique<Clause>(makeExpr(literals[11]), makeExpr(literals[7]), Operator_e::Implies);
				puzzle.applyClue(*c1);
				puzzle.applyClue(*c2);

				Assert::IsTrue(!puzzle.solved());
			}

			TEST_METHOD(Puzzle_ApplyCluesSolve) {
				vector<string> items1{ "Hamlet", "Macbeth", "Romeo & Juliet" };
				vector<string> items2{ "Stephen Colbert", "John Oliver", "Jordan Klepper" };

				vector<Category> categories{
					Category{ "Shakespearean Tragedies", items1.cbegin(), items1.cend() },
					Category{ "Daily Show Correspondents", items2.cbegin(), items2.cend() }
				};
				Puzzle puzzle{ "Yet Again Another Puzzle Again", "...nah no description needed",
					categories.cbegin(), categories.cend() };

				size_t num = items1.size();
				vector<Literal> literals;
				literals.reserve(puzzle.numVariables());
				for (size_t i = 0; i < num; ++i) {
					for (size_t j = 0; j < num; ++j) {
						literals.emplace_back(i * num + j, items1[i] + "|" + items2[j]);
					}
				}

				auto c1 = make_unique<Clause>(makeExpr(literals[0]), makeExpr(literals[2]), Operator_e::Xor);
				auto c2 = makeExpr(literals[6], true);
				auto c3 = make_unique<Clause>(
					make_unique<Clause>(makeExpr(literals[4]), makeExpr(literals[7]), Operator_e::Or),
					makeExpr(literals[5]), Operator_e::Implies);

				puzzle.applyClue(*c1);
				puzzle.applyClue(*c2);
				puzzle.applyClue(*c3);
				Assert::IsTrue(puzzle.solved());
			}
	};
}