#ifndef CORE_H
#define CORE_H

#include <string>



#define DBG defined(_DEBUG) || defined(CHITES_DEBUG)

#if DBG
#include <iostream>
#endif // DEBUG     

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

#include <windows.h>
#include <userenv.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <atlconv.h>

#include <fstream>


#include "Domain.h"

class Core
{
public:
	Core();
	void runProcess(std::wstring aName, std::wstring aInputFilePath, std::wstring aOutputFilePath);

public:
	HANDLE mInputHandle;
	HANDLE mOutputHandle;

	PROCESS_INFORMATION mProcessInfo;
	STARTUPINFOW mStartupInfo;

	void IORedirection(std::wstring& aInputPath, std::wstring& aOutputPath);
	long long getMaxMemoryUsage(PROCESS_INFORMATION&, long long);
	long long getCurrentMemoryUsage(HANDLE&);
	DWORD getExitCode(HANDLE&);
	bool killProcess(PROCESS_INFORMATION&);
	long long getMillisecondsNow();
};

#endif //CORE_H