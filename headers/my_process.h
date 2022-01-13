#ifndef MY_PROCESS_H
#define MY_PROCESS_H

//--------------------------------------------------------------------------------

#define MAX_TIME_LIMIT 5000

#include <string>
#include <future>
#include <iostream>

#include "domain.h"

#ifdef BILL_WINDOWS
#include <windows.h>
#include <userenv.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <atlconv.h>
#include <atlalloc.h>
#include <shlwapi.h>
#include <cstdint> 
#endif

class MyProcess
{
public:
	MyProcess(std::string& aName, std::string& aParameters);
	virtual ~MyProcess();

	void run(uint_16 aTimeLimit = MAX_TIME_LIMIT);
	std::pair<uint_64, uint_64> runWithLimits(uint_64 aTimeLimit, 
		uint_64 aMemoryLimit);

protected:
public:
#ifdef BILL_WINDOWS
	STARTUPINFOA mStartupInfo;
#endif // BILL_WINDOWS

	void create(std::string& aName, std::string& aParameters);

	virtual void IORedirection() = 0;
	virtual void closeHandles() = 0;

private:
#ifdef BILL_WINDOWS
	PROCESS_INFORMATION mProcessInfo;
#endif // BILL_WINDOWS
	std::future<long long> mFuture;

	long long getMillisecondsNow();

	long long getCurrentMemoryUsage(HANDLE&);
	long long getMaxMemoryUsage(PROCESS_INFORMATION&, long long);

	DWORD getExitCode(HANDLE&);
	bool killProcess(PROCESS_INFORMATION&);
};

//--------------------------------------------------------------------------------

#endif // MY_PROCESS_H