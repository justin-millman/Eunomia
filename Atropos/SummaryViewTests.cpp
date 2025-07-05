#include "stdafx.h"												// for UnitTesting framework
#include "CppUnitTest.h"										// for UnitTesting framework
#include "ViewTestReader.h"										// for View testing framework

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

static const char* kViewPrefix = "Summary";
static const char* kScenarioSuffix = "_In";
static const char* kCorrectSuffix = "_Out";


namespace Atropos {
	TEST_CLASS(SummaryTester) {
		public:
			TEST_METHOD(SummaryView_NoGroups) {
				const char* testFileName = "None";
				bool res = testSummaryView(buildTestName(kViewPrefix, testFileName, kScenarioSuffix),
					buildTestName(kViewPrefix, testFileName, kCorrectSuffix));

				Assert::IsTrue(res);
			}

			TEST_METHOD(SummaryView_ManySmallGroups) {
				const char* testFileName = "Many";
				bool res = testSummaryView(buildTestName(kViewPrefix, testFileName, kScenarioSuffix),
					buildTestName(kViewPrefix, testFileName, kCorrectSuffix));

				Assert::IsTrue(res);
			}

			TEST_METHOD(SummaryView_FewMidGroups) {
				const char* testFileName = "NotFull";
				bool res = testSummaryView(buildTestName(kViewPrefix, testFileName, kScenarioSuffix),
					buildTestName(kViewPrefix, testFileName, kCorrectSuffix));

				Assert::IsTrue(res);
			}

			TEST_METHOD(SummaryView_Complete) {
				const char* testFileName = "Full";
				bool res = testSummaryView(buildTestName(kViewPrefix, testFileName, kScenarioSuffix),
					buildTestName(kViewPrefix, testFileName, kCorrectSuffix));

				Assert::IsTrue(res);
			}
	};
}