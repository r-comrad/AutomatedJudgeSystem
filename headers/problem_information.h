#ifndef PROBLEM_INFORMATION_H
#define PROBLEM_INFORMATION_H

#include "domain.h"

struct ProblemInformation
{
public:
	int mID;
	int mContestID;

	uint_64 mTimeLimit;
	uint_64 mMemoryLimit;
	std::string mSolutionFileName;
	std::string mCheckerFileName;

	int mTestsCount;
	bool mTestsAreOver;

	uint_64 mUsedTime;
	uint_64 mUsedMemory;
	std::string mResult;

	ProblemInformation();
	void remakeResultForPasha();
};

#endif // !PROBLEM_INFORMATION_H