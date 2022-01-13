#include "submission_information.h"

SubmissionInformation::SubmissionInformation() :
	id					(0),
	mContestID			(0),

	mTimeLimit			(0),
	mMemoryLimit		(0),
	mSolutionFileName	("NUN NAME"),
	mCheckerFileName	("NUN NAME"),

	mTestsCount			(0),
	mTestsAreOver		(false),

	mUsedTime			(0),
	mUsedMemory			(0),
	mResult				("")
{}

void
SubmissionInformation::remakeResultForPasha()
{
	mResult.resize(2);
	if (mResult == "wa") mResult = "WA";
	else if (mResult == "ok") mResult = "OK";
	else if (mResult == "tle") mResult = "TL";
	else if (mResult == "mle") mResult = "ML";
	else if (mResult == "pe") mResult = "PE";
	else if (mResult == "") mResult = "ERROR";
}