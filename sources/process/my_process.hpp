#ifndef MY_PROCESS_H
#define MY_PROCESS_H

//--------------------------------------------------------------------------------
//				CHILD PROCESS CREATION IMPLIMENTATION 
//--------------------------------------------------------------------------------

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <vector>
#include <iostream>

#include "domain/type.hpp"
#include "domain/my_string.hpp"
#include "domain/error_message.hpp"
#include "domain/path.hpp"
#include "domain/my_pair.hpp"

//--------------------------------------------------------------------------------
// PROCESS CREATION CONSTANTS 
//--------------------------------------------------------------------------------
#ifdef BILL_WINDOWS
#define MAX_TIME_LIMIT 5000
#elif defined(LINUS_LINUX)
#define MAX_TIME_LIMIT 5
#endif 

#define MAX_MEMORY_LIMIT 5000


#define KILLING_PROCESS_TIME_VALUE (uint_64(1e18))
#define KILLING_PROCESS_MEMORY_VALUE (uint_64(1e18))

//--------------------------------------------------------------------------------

#ifdef BILL_WINDOWS
	// BILL_WINDOWS
#define _CRT_SECURE_NO_WARNINGS
#include <future>
#include <windows.h>
#include <userenv.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <atlconv.h>
#include <atlalloc.h>
#include <shlwapi.h>
#include <cstdint>

#include "CPUTime/getCPUTime.hpp"
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
	\brief Base process constructor that initialize time and memory 
	usage limits for child process.
	(all child process have max time and memory usage limits
	that defined in MAX_TIME_LIMIT and MAX_MEMORY_LIMIT)
	\param aParameters Child process parameters for execution.
	\param aTimeLimit Child process maximum time usage.
	\param aMemoryLimit Child process maximum memory usage.
	*/
	MyProcess
	(
		const std::vector<char*>& aParameters,
		uint_64 aTimeLimit = MAX_TIME_LIMIT,
		uint_64 aMemoryLimit = 1e9
	);
	~MyProcess();

	/*
	\brief Executing the child process without time and memory 
	usage evaluation.
	\return Returns true if the process is completed successfully.
	*/
	bool run();

	/*
	\brief Executing the child process with time and memory 
	usage evaluation.
	\return Returns the time and memory used by the process.
	*/
	std::pair<uint_64, uint_64> runWithLimits();

protected:
	/*
	\brief Sets time and memory usage limits.
	\param aTimeLimit Child process maximum time usage.
	\param aMemoryLimit Child process maximum memory usage.
	*/
	void setLimits(uint_64 aTimeLimit, uint_64 aMemoryLimit);

	/*
	\brief Create a child process with the specified parameters.
	\param aParameters Child process parameters for execution.
	*/
	virtual void create(const std::vector<char*>& aParameters);

	/*
	\brief Redirecting input and output streams for a child process.
	*/
	virtual void IORedirection() = 0;

	/*
	\brief Closes the input and output handler for the child process.
	(finishing of communication with the child process)
	*/
#if		defined(BILL_WINDOWS)
	virtual void closeHandles() = 0;
#endif // BILL_WINDOWS

#ifdef BILL_WINDOWS
protected:
	STARTUPINFOA mStartupInfo;
	PROCESS_INFORMATION mProcessInfo;

private:
	std::future<long long> mFuture;

	long long getMillisecondsNow();

	long long getCurrentMemoryUsage(HANDLE&);
	long long getMaxMemoryUsage(PROCESS_INFORMATION&, long long);

	DWORD getExitCode(HANDLE&);
	bool killProcess(PROCESS_INFORMATION&);

#elif defined(LINUS_LINUX)
	int mChildPID;
#endif 

private:
	uint_64 mTimeLimit;
	uint_64 mMemoryLimit;
};

//--------------------------------------------------------------------------------

#endif // MY_PROCESS_H