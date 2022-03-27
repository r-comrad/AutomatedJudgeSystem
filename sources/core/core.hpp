#ifndef CORE_H
#define CORE_H

#define _CRT_SECURE_NO_WARNINGS

#include <map>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>

#include "database/database_query.hpp"

#include "process/file_process.hpp"
#include "process/pipe_process.hpp"

#include "structs/testlib_message.hpp"
#include "structs/problem_information.hpp"
#include "structs/submission_information.hpp"

#include "core/compiler.hpp"

class Core
{
public:
	Core(std::string aDatabasePath);
	~Core();
	void run(int aID);

	//	enum DataStructure {FILES = 0, MAGIC_IVAN = 1};

private:


	std::vector<std::thread*> mChecks;
	std::vector<std::mutex> mChecksMutexs;
	std::vector<SubmissionInformation> mChecksInfo;

	std::vector<char*> mSolutionParameters;
	std::vector<char*> mCheckerParameters;

	std::mutex mGetTestLock;

	uint_32 mFinishedTest;

	MDatabaseQuery mDBQ;
	ProblemInformation mProblemInfo;
    std::mutex mProblemInfoLock;

	void check();
    bool resultEvoluation(int aCheckNum);
	//void fileTesting(uint_32 aTestNum, std::string& aSolutionName, std::string& aCheckerName);
	void pipesTesting(int aThreadNum);
	//void pipesTesting(int aThreadNum, std::string aSolutionName, std::string aCheckerName);
};

#endif //CORE_H