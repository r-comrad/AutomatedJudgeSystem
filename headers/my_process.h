#ifndef MY_PROCESS_H
#define MY_PROCESS_H

//--------------------------------------------------------------------------------

#define MAX_TIME_LIMIT 5000

#include <string>
#include <iostream>

#include "domain.h"

#ifdef BILL_WINDOWS
#include <future>

#include <windows.h>
#include <userenv.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <atlconv.h>
#include <atlalloc.h>
#include <shlwapi.h>
#include <cstdint>
#else
#include <wait.h>
#include <unistd.h>
#endif

class MyProcess
{
public:
	MyProcess();
	virtual ~MyProcess();

	bool run(uint_16 aTimeLimit = MAX_TIME_LIMIT);
	std::pair<uint_64, uint_64> runWithLimits(uint_64 aTimeLimit, 
		uint_64 aMemoryLimit);

protected:
#ifdef BILL_WINDOWS
	STARTUPINFOA mStartupInfo;
#endif // BILL_WINDOWS

	//void create(const std::string& aName, const std::string& aParameters);
	//void create(std::string aName, std::string aParameters);
    void create(const std::vector<char*>& aParameters);

	virtual void IORedirection() = 0;
	virtual void closeHandles() = 0;

private:
#ifdef BILL_WINDOWS
	PROCESS_INFORMATION mProcessInfo;

	std::future<long long> mFuture;

	long long getMillisecondsNow();

	long long getCurrentMemoryUsage(HANDLE&);
	long long getMaxMemoryUsage(PROCESS_INFORMATION&, long long);

	DWORD getExitCode(HANDLE&);
	bool killProcess(PROCESS_INFORMATION&);
#endif // BILL_WINDOWS
};

//--------------------------------------------------------------------------------

#endif // MY_PROCESS_H