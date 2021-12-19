#ifndef RUN_H
#define RUN_H

#include <string>

#include <windows.h>
#include <userenv.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <atlconv.h>

#include "Domain.h"

#include <cstdint> 
#include <future>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <thread>

#include <windows.h>
#include <stdio.h>
#include <atlbase.h>
#include <atlalloc.h>
#include <shlwapi.h>

//#include <windows.h>
#include <userenv.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <atlconv.h>

#include <fstream>


#include "domain.h"

class Process
{
public:
	enum IOType { NONE = 0, FILES = 1, PIPES = 2/*, MIXED = 3 */};
	enum PypeType { ZERO = 0, NO_ZERO = 1};

	Process();
	void run();
	std::pair<uint_64, uint_64> runWithLimits(uint_64 aTimeLimit, uint_64 aMemoryLimit);
	
	void create(std::wstring aName, std::wstring aParameters);

	void IORedirection(IOType aType, std::wstring aInputPath = L"", std::wstring aOutputPath = L"");

	void readPipe(std::string& result);
	void writePipe(std::string aMessage, PypeType aType = ZERO);

	void closeIO();

private:
	//HANDLE mInputHandle;
	//HANDLE mOutputHandle;
	void IORedirectionToPipes();
	void redirectToFile(HANDLE aFileHandle, std::wstring aFilePath);

	HANDLE mThisSTDIN;
	HANDLE mThisSTDOUT;
	HANDLE mChildSTDIN;
	HANDLE mChildSTDOUT;

	PROCESS_INFORMATION mProcessInfo;
	STARTUPINFOW mStartupInfo;
	std::future<long long> mFuture;

	long long getMaxMemoryUsage(PROCESS_INFORMATION&, long long);
	long long getCurrentMemoryUsage(HANDLE&);
	DWORD getExitCode(HANDLE&);
	bool killProcess(PROCESS_INFORMATION&);
	long long getMillisecondsNow();
};

#endif //CORE_H