#ifndef SUBMISSION_STRUCT_H
#define SUBMISSION_STRUCT_H

class SubmissionInformation
{
public:
	int id;
	uint_64 mTimeLimit;
	uint_64 mMemoryLimit;
	std::wstring mSolutionFileName;
	std::wstring mCheckerFileName;
	int mContestID;
	int mTestsCount;
};

#endif //SUBMISSION_STRUCT_H