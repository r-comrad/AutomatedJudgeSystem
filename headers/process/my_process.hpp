#ifndef MY_PROCESS_H
#define MY_PROCESS_H

//--------------------------------------------------------------------------------
//				CHILD PROCESS CREATION IMPLIMENTATION 
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
// PROCESS CREATION CONSTANTS 
//--------------------------------------------------------------------------------
#define MAX_TIME_LIMIT 5000

#define KILLING_PROCESS_TIME_VALUE (uint_64(1e18))
#define KILLING_PROCESS_MEMORY_VALUE (uint_64(1e18))

//--------------------------------------------------------------------------------

#include <string>
#include <vector>
#include <iostream>

#include "domain/domain.hpp"
#include "domain/my_strings.hpp"
#include "domain/errors.hpp"
#include "domain/paths.hpp"
#include "domain/pair.hpp"

#ifdef BILL_WINDOWS
	// BILL_WINDOWS
#include <future>
#include <windows.h>
#include <userenv.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <atlconv.h>
#include <atlalloc.h>
#include <shlwapi.h>
#include <cstdint>
#include "cputime/getCPUTime.h"
#define _CRT_SECURE_NO_WARNINGS
	// !BILL_WINDOWS
#elif defined(LINUS_LINUX)
#include <wait.h>
#include <unistd.h>
#include <fcntl.h>
	// !LINUS_LINUX
#endif 


//--------------------------------------------------------------------------------

class MyProcess
{
public:
	enum PypeType { ZERO = 0, NO_ZERO = 1 };

	/*
	\brief Base process constructor that initialize time and memory limits.
	(all created process have max time execution time limit
	that defined in MAX_TIME_LIMIT define)
	\param aParameters Process paramemeters for execution.
	\param aTimeLimit Process time limit.
	\param aMemoryLimit Process memory limit.
	*/
	MyProcess
	(
		const std::vector<char*>& aParameters,
#ifdef BILL_WINDOWS
		uint_64 aTimeLimit = 1e3,
#elif defined(LINUS_LINUX)
		uint_64 aTimeLimit = 1e1,
#endif 
		uint_64 aMemoryLimit = 1e9
	);
	~MyProcess();

	/*
	\brief Execute process without time and memory evaluation. 
	
	\param aParameters Process paramemeters for execution.
	\param aTimeLimit Process time limit.
	\param aMemoryLimit Process memory limit.
	*/
	//bool run(uint_16 aTimeLimit = MAX_TIME_LIMIT);
	bool run();
	std::pair<uint_64, uint_64> runWithLimits();

protected:
	uint_64 mTimeLimit;
	uint_64 mMemoryLimit;

	void setLimits(uint_64 aTimeLimit, uint_64 aMemoryLimit);

	enum ProcessType { PIPE = 0, FILE = 1 };

#ifdef BILL_WINDOWS
	// BILL_WINDOWS
	STARTUPINFOA mStartupInfo;
	PROCESS_INFORMATION mProcessInfo;

	std::future<long long> mFuture;

	// !BILL_WINDOWS
#elif defined(LINUS_LINUX)
	// LINUS_LINUX
	int mChildPID;
	// !LINUS_LINUX
#endif 

	virtual void create(const std::vector<char*>& aParameters);

	virtual void IORedirection() = 0;
	virtual void closeHandles() = 0;

#ifdef BILL_WINDOWS
	long long getMillisecondsNow();

	long long getCurrentMemoryUsage(HANDLE&);
	long long getMaxMemoryUsage(PROCESS_INFORMATION&, long long);

	DWORD getExitCode(HANDLE&);
	bool killProcess(PROCESS_INFORMATION&);
#endif // !BILL_WINDOWS

private:
	ProcessType mType;
};

//--------------------------------------------------------------------------------

#endif // MY_PROCESS_H