#ifndef SUBMISSION_INFO_HPP
#define SUBMISSION_INFO_HPP

#include "domain/type.hpp"
#include "domain/string.hpp"

struct Limits
{
    uint_64 timeLimit;
    uint_64 memoryLimit;
};

struct SubmissionInfo
{
    int ID;
    int problemID;

    Limits timeMemLim;
    dom::String solutionFileName;
    dom::String checkerFileName;
};

#endif // !SUBMISSION_INFO_HPP