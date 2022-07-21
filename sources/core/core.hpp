#ifndef CORE_H
#define CORE_H

#define _CRT_SECURE_NO_WARNINGS

#include <map>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <list>

#include "database/database_query.hpp"

#include "process/file_process.hpp"
#include "process/pipe_windows_process.hpp"

#include "compiler.hpp"
#include "test.hpp"

class Core
{
public:
	Core(std::string aDatabasePath) noexcept;
	~Core();
	void run(int aID) noexcept;

private:
    std::list<Test> mTests;
    data::DatabaseQuery mDBQ;

    std::string mFinalVerdict;
    //uint_32 mFinalTestNum;
    uint_64 mFinalTime;
    uint_64 mFinalMemory;

    std::shared_ptr<proc::Process> prepareSolutionProcess(SubmissionInfo& aSubInfo)    noexcept;
    std::shared_ptr<proc::Process> prepareCheckerProcess(SubmissionInfo& aSubInfo)     noexcept;
	void check(uint_64 aID) noexcept;
};

#endif //CORE_H