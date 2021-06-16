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

class Core
{
	HANDLE mInputHandle;
	HANDLE mOutputHandle;

	PROCESS_INFORMATION mProcessInfo;

	STARTUPINFOW mStartupInfo;

	Core();

	void runProcess(std::wstring aName, std::wstring aInputFilePath, std::wstring aOutputFilePath);
	void IORedirection(std::wstring& aInputPath, std::wstring& aOutputPath);
};

#endif //CORE_H