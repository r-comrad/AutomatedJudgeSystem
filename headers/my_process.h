#ifndef MY_PROCESS_H
#define MY_PROCESS_H

//--------------------------------------------------------------------------------

#define MAX_TIME_LIMIT 5000

#include <string>
#include <vector>
#include <iostream>

#include "domain.h"

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
	// !BILL_WINDOWS
#elif defined(LINUS_LINUX)
#include <wait.h>
#include <unistd.h>
#include <fcntl.h>
	// !LINUS_LINUX
#endif 

class MyProcess
{
public:
	enum PypeType { ZERO = 0, NO_ZERO = 1 };

	MyProcess
	(
		const std::vector<char*>& aParameters,
#ifdef BILL_WINDOWS
		// BILL_WINDOWS
		uint_64 aTimeLimit = 1e1,
		// !BILL_WINDOWS
#elif defined(LINUS_LINUX)
		// LINUS_LINUX
		uint_64 aTimeLimit = 1e3,
		// !LINUS_LINUX
#endif 
		uint_64 aMemoryLimit = 1e9
	);
	~MyProcess();

	bool run(uint_16 aTimeLimit = MAX_TIME_LIMIT);
	std::pair<uint_64, uint_64> runWithLimits();

	void setLimits(uint_64 aTimeLimit, uint_64 aMemoryLimit);



	void readPipe(std::string& result);
	void writePipe(std::string& aMessage, PypeType aType = ZERO);

private:
	uint_64 mTimeLimit;
	uint_64 mMemoryLimit;

#ifdef BILL_WINDOWS
	// BILL_WINDOWS
	STARTUPINFOA mStartupInfo;
	PROCESS_INFORMATION mProcessInfo;

	HANDLE mThisSTDIN;
	HANDLE mThisSTDOUT;
	HANDLE mChildSTDIN;
	HANDLE mChildSTDOUT;

	std::future<long long> mFuture;

	// !BILL_WINDOWS
#elif defined(LINUS_LINUX)
	// LINUS_LINUX
	int mPipeA[2];
	int mPipeB[2];
	int mChildPID;

	// !LINUS_LINUX
#endif 

	void create(const std::vector<char*>& aParameters);

	void IORedirection();
	void closeHandles();

#ifdef BILL_WINDOWS
	long long getMillisecondsNow();

	long long getCurrentMemoryUsage(HANDLE&);
	long long getMaxMemoryUsage(PROCESS_INFORMATION&, long long);

	DWORD getExitCode(HANDLE&);
	bool killProcess(PROCESS_INFORMATION&);
#endif // !BILL_WINDOWS
};

//--------------------------------------------------------------------------------

#endif // MY_PROCESS_H