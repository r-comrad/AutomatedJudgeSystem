#ifndef MY_PROCESS_H
#define MY_PROCESS_H

//--------------------------------------------------------------------------------

#define MAX_TIME_LIMIT 5000

#define KILLING_PROCESS_TIME_VALUE (uint_64(1e18))
#define KILLING_PROCESS_MEMORY_VALUE (uint_64(1e18))

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
		uint_64 aTimeLimit = 1e3,
#elif defined(LINUS_LINUX)
		uint_64 aTimeLimit = 1e1,
#endif 
		uint_64 aMemoryLimit = 1e9
	);
	~MyProcess();

	bool run(uint_16 aTimeLimit = MAX_TIME_LIMIT);
	std::pair<uint_64, uint_64> runWithLimits();

	//void setLimits(uint_64 aTimeLimit, uint_64 aMemoryLimit);



	//void readPipe(std::string& result);
	//void writePipe(std::string& aMessage, PypeType aType = ZERO);

protected:
	uint_64 mTimeLimit;
	uint_64 mMemoryLimit;

	void setLimits(uint_64 aTimeLimit, uint_64 aMemoryLimit);

	enum ProcessType { PIPE = 0, FILE = 1 };

#ifdef BILL_WINDOWS
	// BILL_WINDOWS
	STARTUPINFOA mStartupInfo;
	PROCESS_INFORMATION mProcessInfo;

	//HANDLE mThisSTDIN;
	//HANDLE mThisSTDOUT;
	//HANDLE mChildSTDIN;
	//HANDLE mChildSTDOUT;

	std::future<long long> mFuture;

	// !BILL_WINDOWS
#elif defined(LINUS_LINUX)
	// LINUS_LINUX
	//int mPipeA[2];
	//int mPipeB[2];
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