#ifndef SUBMISSION_INFORMATION_H
#define SUBMISSION_INFORMATION_H

#include <string>

#include "main/flags.hpp"

#include "domain/type.hpp"

struct SubmissionInformation
{
public:
	bool		mIsFinishedTesting;
	uint_64		mUsedTime;
	uint_64		mUsedMemory;
	std::string	mResult;

	SubmissionInformation();
};

#endif // !SUBMISSION_INFORMATION_H