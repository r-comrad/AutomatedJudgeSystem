#ifndef PROBLEM_INFORMATION_H
#define PROBLEM_INFORMATION_H

#include "main/flags.hpp"

#include "domain/type.hpp"
#include "domain/string.hpp"

struct ProblemInformation
{
public:
	int mID;
	int mContestID;

	uint_64 mTimeLimit;
	uint_64 mMemoryLimit;
	str_val mSolutionFileName;
	str_val mCheckerFileName;

	int mTestsCount;
	bool mTestsAreOver;

	uint_64 mUsedTime;
	uint_64 mUsedMemory;
	str_val mResult;

	ProblemInformation();
	void remakeResultForPasha();
};

#endif // !PROBLEM_INFORMATION_H