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
#include <fcntl.h>
#endif

class MyProcess
{
public:
	MyProcess
    (
            const std::vector<char*>& aParameters,
            uint_64 aTimeLimit=1e1,
            uint_64 aMemoryLimit=1e9
    );
	~MyProcess();

	bool run(uint_16 aTimeLimit = MAX_TIME_LIMIT);
	std::pair<uint_64, uint_64> runWithLimits();
    void setLimits(uint_64 aTimeLimit, uint_64 aMemoryLimit);

protected:
#ifdef BILL_WINDOWS
	STARTUPINFOA mStartupInfo;
#endif // BILL_WINDOWS

	//void create(const std::string& aName, const std::string& aParameters);
	//void create(std::string aName, std::string aParameters);
    void create(const std::vector<char*>& aParameters);

	void IORedirection() ;
	void closeHandles() ;

private:
    uint_64 mTimeLimit;
    uint_64 mMemoryLimit;
#ifdef BILL_WINDOWS
	PROCESS_INFORMATION mProcessInfo;

	std::future<long long> mFuture;

	long long getMillisecondsNow();

	long long getCurrentMemoryUsage(HANDLE&);
	long long getMaxMemoryUsage(PROCESS_INFORMATION&, long long);

	DWORD getExitCode(HANDLE&);
	bool killProcess(PROCESS_INFORMATION&);
#endif // BILL_WINDOWS

public:
    enum PypeType { ZERO = 0, NO_ZERO = 1 };

    void readPipe(std::string& result);
    void writePipe(std::string& aMessage, PypeType aType = ZERO);

//private:
//    void IORedirection();
//    void closeHandles();

#ifdef BILL_WINDOWS
    HANDLE mThisSTDIN;
	HANDLE mThisSTDOUT;
	HANDLE mChildSTDIN;
	HANDLE mChildSTDOUT;
#else
    int mPipeA[2];
    int mPipeB[2];
    int mChildPID;
//public:
    //static thread_local bool mIsPaused;
   // static void handleContinueSignal(int sig);
#endif // BILL_WINDOWS
};

//--------------------------------------------------------------------------------

#endif // MY_PROCESS_H