#include "structs/problem_information.hpp"

ProblemInformation::ProblemInformation() :
	mID					(0),
	mContestID			(0),

	mTimeLimit			(0),
	mMemoryLimit		(0),
	mSolutionFileName	(nullptr),
	mCheckerFileName	(nullptr),

	mTestsCount			(0),
	mTestsAreOver		(false),
	
	mUsedTime			(0),
	mUsedMemory			(0),
	mResult				("NUN")
{}

void
ProblemInformation::remakeResultForPasha()
{
	mResult.resize(2);
	if (mResult == "wa")		mResult = "WA";
	else if (mResult == "ok")	mResult = "OK";
	else if (mResult == "tl")	mResult = "TL";
	else if (mResult == "ml")	mResult = "ML";
	else if (mResult == "pe")	mResult = "PE";
	else if (mResult == "NU")	mResult = "ERROR#1";
	else if (mResult == "")		mResult = "ERROR#2";
}