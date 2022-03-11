#ifndef PROBLEM_INFORMATION_H
#define PROBLEM_INFORMATION_H

#include "domain.h"
#include "my_strings.hpp"

struct ProblemInformation
{
public:
	int mID;
	int mContestID;

	uint_64 mTimeLimit;
	uint_64 mMemoryLimit;
	std_string mSolutionFileName;
	std_string mCheckerFileName;

	int mTestsCount;
	bool mTestsAreOver;

	uint_64 mUsedTime;
	uint_64 mUsedMemory;
	std_string mResult;

	ProblemInformation();
	void remakeResultForPasha();
};

#endif // !PROBLEM_INFORMATION_H