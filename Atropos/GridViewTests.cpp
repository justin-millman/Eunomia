#include "stdafx.h"												// for UnitTesting framework
#include "CppUnitTest.h"										// for UnitTesting framework
#include "ViewTestReader.h"										// for View testing framework

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

static const char* kViewPrefix = "Grid";
static const char* kScenarioSuffix = "_In";
static const char* kCorrectSuffix = "_Out";


namespace Atropos {
	TEST_CLASS(GridTester) {
		public:
			TEST_METHOD(GridView_LongestShared) {
				const char* testFileName = "SharedLongest";
				bool res = testGridView(buildTestName(kViewPrefix, testFileName, kScenarioSuffix),
					buildTestName(kViewPrefix, testFileName, kCorrectSuffix));

				Assert::IsTrue(res);
			}
		
			TEST_METHOD(GridView_LongestSeparate) {
				const char* testFileName = "DiffLongest";
				bool res = testGridView(buildTestName(kViewPrefix, testFileName, kScenarioSuffix),
					buildTestName(kViewPrefix, testFileName, kCorrectSuffix));

				Assert::IsTrue(res);
		}
	};
}