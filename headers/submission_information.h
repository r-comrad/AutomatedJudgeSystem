#ifndef SUBMISSION_STRUCT_H
#define SUBMISSION_STRUCT_H

#include "domain.h"

struct SubmissionInformation
{
public:
	int id;
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

	SubmissionInformation();
	void remakeResultForPasha();
};

#endif //SUBMISSION_STRUCT_H