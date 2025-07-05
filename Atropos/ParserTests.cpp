#include "stdafx.h"												// for UnitTesting framework
#include "CppUnitTest.h"										// for UnitTesting framework
#include "../Eunomia/Expression.h"								// for Expression
#include "../Eunomia/Parser.h"									// for Parser
#include "../Eunomia/Utility.h"									// for constant char symbols
#include <set>													// for set
#include <sstream>												// for stringstream
#include <string>												// for string
#include <vector>												// for vector

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using std::string;
using std::stringstream;
using std::vector; using std::set;


namespace Atropos {
	TEST_CLASS(ParserTester) {
		public:
			TEST_METHOD(Parser_SimpleConstruct) {
				Parser parser{};
				Assert::IsTrue(!parser.isParseString("("));
				Assert::IsTrue(!parser.isParseString("+"));
				Assert::IsTrue(!parser.isParseString("Crocodile"));
			}

			TEST_METHOD(Parser_VectorConstruct) {
				vector<string> strs{ "Drone", "Helicopter", "Aeroplane", "PaperAirplane",
					"HangGlider" };
				Parser parser{ strs.cbegin(), strs.cend() };

				for (const auto& str : strs) {
					Assert::IsTrue(parser.isParseString(str));
				}
			}

			TEST_METHOD(Parser_ArrayConstruct) {
				string strs[]{ "IndoEuropean", "Romance", "Semitic", "Slavic", "Austronesian",
					"Bantu", "Germanic", "Gaelic", "Uto-Aztecan" };
				size_t size = sizeof(strs) / sizeof(string);

				Parser parser{ strs, strs + size };
				for (size_t i = 0; i < size; ++i) {
					Assert::IsTrue(parser.isParseString(strs[i]));
				}
			}

			TEST_METHOD(Parser_SetConstruct) {
				set<string> strs{ "Pikachu", "Electabuzz", "Voltorb", "Manectric",
					"Ampharos", "Luxray" };
				Parser parser{ strs.cbegin(), strs.cend() };

				for (const auto& str : strs) {
					Assert::IsTrue(parser.isParseString(str));
				}
			}

			TEST_METHOD(Parser_AddLegalString) {
				Parser parser{};
				vector<string> strs{ "Volcanology", "Immunology", "Ornithology" };

				try {
					for (const auto& str : strs) {
						parser.addParseString(str, 4);
					}
					for (const auto& str : strs) {
						Assert::IsTrue(parser.isParseString(str));
					}
				}
				catch (...) {
					Assert::IsTrue(false);
				}
			}

			TEST_METHOD(Parser_AddStringSpace) {
				Parser parser{};
				
				try {
					parser.addParseString("Bernie Sanders", 0);
					Assert::IsTrue(false);
				}
				catch (ParseException) {
					Assert::IsTrue(true);
				}
				catch (...) {
					Assert::IsTrue(false);
				}
			}

			TEST_METHOD(Parser_AddGroupingString) {
				Parser parser{};
				vector<string> illegals{ "Gond)ola", "(Mezzanine", "Se7en]", "Coca[ine",
					"Dyspros{ium", "Rhododendron}" };

				for (const auto& str : illegals) {
					try {
						parser.addParseString(str, 0);
						Assert::IsTrue(false);
					}
					catch (ParseException) {
						Assert::IsTrue(true);
					}
					catch (...) {
						Assert::IsTrue(false);
					}
				}
			}

			TEST_METHOD(Parser_AddOperatorString) {
				Parser parser{};

				vector<string> illegals{ string{ kAndSymbol } +"Sodomy",
					"Proli" + string {kNotSymbol } +"feration",
					"Ba" + string { kOrSymbol } +"llpark",
					"Firefl" + string{ kXorSymbol } + "y",
					"SydneyOpera" + string { kImpliesSymbol} + "House" };

				for (const auto& str : illegals) {
					try {
						parser.addParseString(str, 0);
						Assert::IsTrue(false);
					}
					catch (ParseException) {
						Assert::IsTrue(true);
					}
					catch (...) {
						Assert::IsTrue(false);
					}
				}
			}

			TEST_METHOD(Parser_RemoveString) {
				vector<string> strs{ "Ice", "Steam", "WaterVapor" };
				Parser parser{ strs.cbegin(), strs.cend() };

				try {
					for (const auto& str : strs) {
						parser.removeParseString(str);
					}
					for (const auto& str : strs) {
						Assert::IsTrue(!parser.isParseString(str));
					}
				}
				catch (...) {
					Assert::IsTrue(false);
				}
			}

			TEST_METHOD(Parser_RemoveNonexistentString) {
				Parser parser{};

				try {
					parser.removeParseString("Guadalajara,Mexico");
					Assert::IsTrue(false);
				}
				catch (ParseException) {
					Assert::IsTrue(true);
				}
				catch (...) {
					Assert::IsTrue(false);
				}
			}

			TEST_METHOD(Parser_ParseLiteral) {
				const Parser& parser = getParsingParser();
				const string str = "C";
			
				try {
					auto expr = parser.parse(str);

					stringstream sstream{};
					sstream << *expr;
					Assert::IsTrue(sstream.str() == str);
				}
				catch (...) {
					Assert::IsTrue(false);
				}
			}

			TEST_METHOD(Parser_ParseNegatedLiteral) {
				const Parser& parser = getParsingParser();
				const string str = "(" + string{ kNotSymbol } + "A)";

				try {
					auto expr = parser.parse(str);

					stringstream sstream{};
					sstream << *expr;
					Assert::IsTrue(sstream.str() == string{ kNotSymbol } +"A");
				}
				catch (...) {
					Assert::IsTrue(false);
				}
			}

			TEST_METHOD(Parser_ParseMultiNegatedNot) {
				const Parser& parser = getParsingParser();
				const string str = "(" + string{ kNotSymbol } + kNotSymbol + kNotSymbol + kNotSymbol + "C)";

				try {
					auto expr = parser.parse(str);

					stringstream sstream{};
					sstream << *expr;
					Assert::IsTrue(sstream.str() == "C");
				}
				catch (...) {
					Assert::IsTrue(false);
				}
			}

			TEST_METHOD(Parser_ParseMultiNegatedYes) {
				const Parser& parser = getParsingParser();
				const string str = "(" + string{ kNotSymbol } + kNotSymbol + kNotSymbol + "C)";

				try {
					auto expr = parser.parse(str);

					stringstream sstream{};
					sstream << *expr;
					Assert::IsTrue(sstream.str() == string{ kNotSymbol } + "C");
				}
				catch (...) {
					Assert::IsTrue(false);
				}
			}

			TEST_METHOD(Parser_ParseSimpleAnd) {
				const Parser& parser = getParsingParser();
				const string str = "(C " + string{ kAndSymbol } + " E)";
				
				try {
					auto expr = parser.parse(str);

					stringstream sstream{};
					sstream << *expr;
					Assert::IsTrue(sstream.str() == str);
				}
				catch (...) {
					Assert::IsTrue(false);
				}
			}

			TEST_METHOD(Parser_ParseNegatedAnd) {
				const Parser& parser = getParsingParser();
				const string str = string{ kNotSymbol } +"(B " + string{ kAndSymbol } +" C)";
				
				try {
					auto expr = parser.parse(str);

					stringstream sstream{};
					sstream << *expr;

					stringstream sresult{};
					sresult << "(" << kNotSymbol << "B" << " " << kOrSymbol << " " << kNotSymbol << "C)";
					Assert::IsTrue(sstream.str() == sresult.str());
				}
				catch (...) {
					Assert::IsTrue(false);
				}
			}

			TEST_METHOD(Parser_ParseSimpleOr) {
				const Parser& parser = getParsingParser();
				const string str = "E " + string{ kOrSymbol } + " B";
				
				try {
					auto expr = parser.parse(str);

					stringstream sstream{};
					sstream << *expr;
					Assert::IsTrue(sstream.str() == "(" + str + ")");
				}
				catch (...) {
					Assert::IsTrue(false);
				}
			}

			TEST_METHOD(Parser_ParseNegatedOr) {
				const Parser& parser = getParsingParser();
				const string str = string{ kNotSymbol } +"(D " + string{ kOrSymbol } +" A)";
				
				try {
					auto expr = parser.parse(str);

					stringstream sstream{};
					sstream << *expr;

					stringstream sresult{};
					sresult << "(" << kNotSymbol << "D" << " " << kAndSymbol << " " << kNotSymbol << "A)";
					Assert::IsTrue(sstream.str() == sresult.str());
				}
				catch (...) {
					Assert::IsTrue(false);
				}
			}

			TEST_METHOD(Parser_ParseSimpleImplies) {
				const Parser& parser = getParsingParser();
				const string str = "(A " + string{ kImpliesSymbol } + " D)";
				
				try {
					auto expr = parser.parse(str);

					stringstream sstream{};
					sstream << *expr;
					Assert::IsTrue(sstream.str() == str);
				}
				catch (...) {
					Assert::IsTrue(false);
				}
			}

			TEST_METHOD(Parser_ParseNegatedImplies) {
				const Parser& parser = getParsingParser();
				const string str = string{ kNotSymbol } +"(C " + string{ kImpliesSymbol } +" B)";
				
				try {
					auto expr = parser.parse(str);

					stringstream sstream{};
					sstream << *expr;

					stringstream sresult{};
					sresult << "(C" << " " << kAndSymbol << " " << kNotSymbol << "B)";
					Assert::IsTrue(sstream.str() == sresult.str());
				}
				catch (...) {
					Assert::IsTrue(false);
				}
			}

			TEST_METHOD(Parser_ParseSimpleXor) {
				const Parser& parser = getParsingParser();
				const string str = "A " + string{ kXorSymbol } + " D";
				
				try {
					auto expr = parser.parse(str);

					stringstream sstream{};
					sstream << *expr;
					Assert::IsTrue(sstream.str() == "(" + str + ")");
				}
				catch (...) {
					Assert::IsTrue(false);
				}
			}

			TEST_METHOD(Parser_ParseNegatedXor) {
				const Parser& parser = getParsingParser();
				const string str = string{ kNotSymbol } + "(A " + string{ kXorSymbol } + " B)";
				
				try {
					auto expr = parser.parse(str);

					stringstream sstream{};
					sstream << *expr;

					stringstream sresult{};
					sresult << "("
						<< "(A " << kAndSymbol << " B)"
						<< " " << kOrSymbol << " "
						<< "(" << kNotSymbol << "A " << kAndSymbol << " " << kNotSymbol << "B))";
					Assert::IsTrue(sstream.str() == sresult.str());
				}
				catch (...) {
					Assert::IsTrue(false);
				}
			}

			TEST_METHOD(Parser_ParseGroupedClause) {
				const Parser& parser = getParsingParser();

				try {
					const string str = "({A " + string{ kAndSymbol } + " B} "
						+ kImpliesSymbol + " [" + kNotSymbol + "C " + kOrSymbol + " D])";
					auto expr = parser.parse(str);

					stringstream sstream{};
					sstream << *expr;

					const string res = "((A " + string{ kAndSymbol } +" B) "
						+ kImpliesSymbol + " (" + kNotSymbol + "C " + kOrSymbol + " D))";
					Assert::IsTrue(sstream.str() == res);
				}
				catch (...) {
					Assert::IsTrue(false);
				}
			}

			TEST_METHOD(Parser_ParseOvergroupedClause) {
				const Parser& parser = getParsingParser();

				try {
					const string str = "([([[{E " + string{ kXorSymbol } + " " + kNotSymbol
						+ "A}]])])";
					auto expr = parser.parse(str);

					stringstream sstream{};
					sstream << *expr;

					const string res = "(E " + string{ kXorSymbol } +" " + kNotSymbol + "A)";
					Assert::IsTrue(sstream.str() == res);
				}
				catch (...) {
					Assert::IsTrue(false);
				}
			}

			TEST_METHOD(Parser_ParseUngroupedClause) {
				const Parser& parser = getParsingParser();

				try {
					const string str = "A " + string{ kOrSymbol } + " " + kNotSymbol + "Lemon " +
						kImpliesSymbol + " " + kNotSymbol + "B";
					auto expr = parser.parse(str);

					stringstream sstream{};
					sstream << *expr;

					const string res = "((A " + string{ kOrSymbol } + " " + kNotSymbol + "Lemon) " +
						kImpliesSymbol + " " + kNotSymbol + "B)";
					Assert::IsTrue(sstream.str() == res);
				}
				catch (...) {
					Assert::IsTrue(false);
				}
			}

			TEST_METHOD(Parser_ParseNoSpaces) {
				const Parser& parser = getParsingParser();

				try {
					const string str = "({Lemon" + string{ kImpliesSymbol } +"B}" + kXorSymbol
						+ "[C" + kOrSymbol + "A])";
					auto expr = parser.parse(str);

					stringstream sstream{};
					sstream << *expr;

					const string res = "({Lemon " + string{ kImpliesSymbol } +" B} " + kXorSymbol
						+ " [C " + kOrSymbol + " A])";
				}
				catch (...) {
					Assert::IsTrue(false);
				}
			}

			TEST_METHOD(Parser_ParseExcessSpaces) {
				const Parser& parser = getParsingParser();

				try {
					const string str = "  (  A            " + string{ kOrSymbol } +
						"    " + kNotSymbol + "     B      )    ";
					auto expr = parser.parse(str);

					stringstream sstream{};
					sstream << *expr;

					const string res = "(A " + string{ kOrSymbol } +" " + kNotSymbol + "B)";
					Assert::IsTrue(sstream.str() == res);
				}
				catch (...) {
					Assert::IsTrue(false);
				}
			}

			TEST_METHOD(Parser_ParseUnmatchedGroupings) {
				const Parser& parser = getParsingParser();

				try {
					auto expr = parser.parse("((A " + string{ kAndSymbol } +" D) " + kOrSymbol + " E");
					Assert::IsTrue(false);
				}
				catch (ParseException) {
					Assert::IsTrue(true);
				}
				catch (...) {
					Assert::IsTrue(false);
				}
			}

			TEST_METHOD(Parser_ParseMismatchedGroupings) {
				const Parser& parser = getParsingParser();

				try {
					auto expr = parser.parse("[{B " + string{ kAndSymbol } +" E] " + kOrSymbol + " B)");
					Assert::IsTrue(false);
				}
				catch (ParseException) {
					Assert::IsTrue(true);
				}
				catch (...) {
					Assert::IsTrue(false);
				}
			}

			TEST_METHOD(Parser_ParseUnrecognizedString) {
				const Parser& parser = getParsingParser();

				try {
					auto expr = parser.parse(kNotSymbol + "F");
				}
				catch (ParseException) {
					Assert::IsTrue(true);
				}
				catch (...) {
					Assert::IsTrue(false);
				}
			}

			TEST_METHOD(Parser_ParseUnrecognizedOperator) {
				const Parser& parser = getParsingParser();

				try {
					auto expr = parser.parse("A % B");
				}
				catch (ParseException) {
					Assert::IsTrue(true);
				}
				catch (...) {
					Assert::IsTrue(false);
				}
			}

			TEST_METHOD(Parser_ParseEmptyGroup) {
				const Parser& parser = getParsingParser();

				try {
					auto expr = parser.parse("[(A " + string{ kOrSymbol } + " B){}]");
				}
				catch (ParseException) {
					Assert::IsTrue(true);
				}
				catch (...) {
					Assert::IsTrue(false);
				}
			}

			TEST_METHOD(Parser_PrintEmpty) {
				Parser parser{};
				stringstream sstream{};

				sstream << parser;
				Assert::IsTrue(sstream.str() == "There are no parse-strings set in this Parser\n");
			}

			TEST_METHOD(Parser_PrintNonempty) {
				Parser parser{};
				parser.addParseString("GrandCayman", 3);
				parser.addParseString("Aruba", 192);
				parser.addParseString("Trinidad", 15);
				parser.addParseString("Tasmania", 14);
				parser.addParseString("Oahu", 192);
				parser.addParseString("Curacao", 56);
				parser.addParseString("Greenland", 1);

				stringstream sstream{};
				sstream << parser;
				Assert::IsTrue(sstream.str() ==
					"  1: Greenland\n"
					"  3: GrandCayman\n"
					" 14: Tasmania\n"
					" 15: Trinidad\n"
					" 56: Curacao\n"
					"192: Aruba\n"
					"192: Oahu\n");
			}

		private:
			const Parser& getParsingParser() const {
				vector<string> strs{ "A", "B", "C", "D", "E", "Lemon" };
				static Parser parser{ strs.cbegin(), strs.cend() };
				return parser;
			}
	};
}