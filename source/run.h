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

#include <windows.h>
#include <userenv.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <atlconv.h>

#include <fstream>


#include "domain.h"

class Run
{
public:
	Run();
	void runProcess(std::wstring aName, std::wstring aInputFilePath, std::wstring aOutputFilePath, std::wstring aParameters);
	void createProcess();


public:
	HANDLE mInputHandle;
	HANDLE mOutputHandle;

	PROCESS_INFORMATION mProcessInfo;
	STARTUPINFOW mStartupInfo;

	void IORedirection(std::wstring& aInputPath, std::wstring& aOutputPath);

};

#endif //CORE_H