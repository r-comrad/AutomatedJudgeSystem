#ifndef SUBMISSION_INFO_HPP
#define SUBMISSION_INFO_HPP

#include "domain/type.hpp"
#include "domain/string.hpp"

struct SubmissionInfo
{
public:
	int ID;
	int problemID;

	uint_64 timeLimit;
	uint_64 memoryLimit;
	dom::String solutionFileName;
	dom::String checkerFileName;
};

#endif // !SUBMISSION_INFO_HPP