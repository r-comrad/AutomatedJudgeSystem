#ifndef CORE_H
#define CORE_H

#include <map>

#include "database_query.h"
#include "process.h"
#include "testlib_message.h"

class Core
{
public:
	Core(std::wstring aDatabasePath);
	void run(int aID);

private:
	enum Language { NUN = 0, MagicCPP, Snake};

	MDatabaseQuery mDBQ;
	SubmissionInformation mSubInfo;

	std::wstring makeExecutable(std::wstring aFileName, std::wstring aOutputName);
	Core::Language getLanguage(std::wstring aFileName);
	std::wstring compile(std::wstring aFileName, std::wstring aOutName, Language aLanguage);

	void check(std::wstring aSolutionName, std::wstring aCheckerName);
	void fileTesting(uint_32 aTestNum, std::wstring aSolutionName, std::wstring aCheckerName);
	void pipesTesting(std::wstring aSolutionName, std::wstring aCheckerName);
};

#endif //CORE_H