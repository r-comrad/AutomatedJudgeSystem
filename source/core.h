#ifndef CORE_H
#define CORE_H

#include <map>

#include "database_query.h"
#include "process.h"
#include "testlib_message.h"

class Core
{
public:
	Core(std::string aDatabasePath);
	void run(int aID);

private:
	enum Language { NUN = 0, MagicCPP, Snake};

	MDatabaseQuery mDBQ;
	SubmissionInformation mSubInfo;

	std::string makeExecutable(std::string aFileName, std::string aOutputName);
	Core::Language getLanguage(std::string aFileName);
	std::string compile(std::string aFileName, std::string aOutName, Language aLanguage);

	void check(std::string aSolutionName, std::string aCheckerName);
	void fileTesting(uint_32 aTestNum, std::string aSolutionName, std::string aCheckerName);
	void pipesTesting(std::string aSolutionName, std::string aCheckerName);
};

#endif //CORE_H