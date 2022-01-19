#ifndef CORE_H
#define CORE_H

#include <map>
#include <queue>
#include <thread>
#include <mutex>

#include "database_query.h"
#include "file_process.h"
#include "pipe_process.h"
#include "testlib_message.h"

class Core
{
public:
	Core(std::string aDatabasePath);
	void run(int aID);

	//	enum DataStructure {FILES = 0, MAGIC_IVAN = 1};

private:
	enum Language 
	{ 
		NUN			=	0, 
		MAGIC_CPP	=	1, 
		SNAKE		=	2
	};

	std::queue<std::thread*> mChecks;

	std::mutex mGetTestLock;
	std::mutex mResultLock;
	std::mutex mMesuareLock;

	MDatabaseQuery mDBQ;
	SubmissionInformation mSubInfo;

	Core::Language getLanguage(std::string aFileName);
	std::string compile(std::string aFileName, std::string aOutName, Language aLanguage);
	std::string makeExecutable(std::string aFileName, std::string aOutputName);

	void check(std::string& aSolutionName, std::string& aCheckerName);
	void fileTesting(uint_32 aTestNum, std::string& aSolutionName, std::string& aCheckerName);
	void pipesTesting(std::string& aSolutionName, std::string& aCheckerName);
};

#endif //CORE_H