#pragma GCC diagnostic ignored "-Wwrite-strings"
#define INT_MAX 2147483647

#include "Run.h"


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

Process::Process() :
    mStartupInfo({ 0 })
{
    //mProcessInfo = 0;
    ZeroMemory(&mProcessInfo, sizeof(PROCESS_INFORMATION));
}

void Process::IORedirection(std::wstring aInputPath, std::wstring aOutputPath)
{
#ifdef _DBG_
    std::cout << "Rederecting input to: " << makeGoodString(aInputPath) << " and output to: " << makeGoodString(aOutputPath) << std::endl;
#endif

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    if (aInputPath != L"")
    {
        mInputHandle = CreateFile(aInputPath.c_str(),
            GENERIC_READ,
            FILE_SHARE_READ,
            &sa,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
    }

    if (aOutputPath != L"")
    {
        mOutputHandle = CreateFile(aOutputPath.c_str(),
            FILE_WRITE_DATA,
            FILE_SHARE_WRITE,
            &sa,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
    }

    //BOOL ret = FALSE;
    //DWORD flags = 0;// CREATE_NO_WINDOW;

    ZeroMemory(&mStartupInfo, sizeof(STARTUPINFO));
    mStartupInfo.cb = sizeof(STARTUPINFO);
    mStartupInfo.dwFlags |= STARTF_USESTDHANDLES;
    if (aInputPath != L"")  mStartupInfo.hStdInput = mInputHandle;
    if (aOutputPath != L"") mStartupInfo.hStdError = mOutputHandle;
    if (aOutputPath != L"") mStartupInfo.hStdOutput = mOutputHandle;
}

void Process::create
(
    std::wstring aName,
    std::wstring aParameters
)
{
#ifdef _DBG_
    std::cout << "Creating process name: " << makeGoodString(aName) << " parameters: " << makeGoodString(aParameters) << std::endl;
#endif

    wchar_t* name = const_cast<wchar_t*>(aName.c_str());
    if (aName == L"") name = NULL;

    wchar_t* cmd = const_cast<wchar_t*>(aParameters.c_str());
    if (aParameters == L"") cmd = NULL;

    mFuture = std::async(std::launch::async, &Process::getMaxMemoryUsage, this, std::ref(mProcessInfo), 1000000);

    if (CreateProcess(
        name,
        cmd,
        NULL,
        NULL,
        TRUE,
        CREATE_UNICODE_ENVIRONMENT | CREATE_SUSPENDED | CREATE_NO_WINDOW,
        NULL,
        NULL,
        &mStartupInfo,
        &mProcessInfo
    ) == FALSE)
    {
#ifdef _DBG_
        std::cout << "!!!!!! ERROR #2: can't start process" << std::endl;
#endif // DEBUG      
    }
}

void Process::run()
{
#ifdef _DBG_
    std::cout << "Runing simple process" << std::endl;
#endif

    ResumeThread(mProcessInfo.hThread);
    WaitForSingleObject(mProcessInfo.hProcess, INFINITE);
    CloseHandle(mInputHandle);
    CloseHandle(mOutputHandle);
}

void Process::run(uint_64 aTimeLimit, uint_64 aMemoryLimit)
{
#ifdef _DBG_
    std::cout << "Runing process with time and memory evaluation" << std::endl;
#endif

    int  reservedTime = 200;
    ResumeThread(mProcessInfo.hThread);
    long long startTime = getMillisecondsNow();
    WaitForSingleObject(mProcessInfo.hProcess, INFINITE);

    if (getExitCode(mProcessInfo.hProcess) == STILL_ACTIVE) //TODO: get 
    {
        killProcess(mProcessInfo); //TODO: kill
    }

    long long endTime = getMillisecondsNow();
    uint_64 timeUsage = endTime - startTime;

    uint_64 memoryUsage = 0;
    memoryUsage = mFuture.get();

#ifdef _DBG_
    std::cout << "time usage: " << endTime - startTime << std::endl;
    std::cout << "memory usage: " << memoryUsage << std::endl;
#endif // DEBUG  

    CloseHandle(mInputHandle);
    CloseHandle(mOutputHandle);
}

long long Process::getMillisecondsNow() {
    static LARGE_INTEGER frequency;
    static BOOL useQpf = QueryPerformanceFrequency(&frequency);
    if (useQpf) {
        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        return (1000LL * now.QuadPart) / frequency.QuadPart;
    }
    else {
        return GetTickCount();
    }
    return 0;
}

long long Process::getCurrentMemoryUsage(HANDLE& hProcess) {
    PROCESS_MEMORY_COUNTERS pmc;
    long long  currentMemoryUsage = 0;

    if (!GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
        return 0;
    }
    currentMemoryUsage = pmc.PeakWorkingSetSize >> 10;

    if (currentMemoryUsage < 0) {
        currentMemoryUsage = INT_MAX >> 10;
    }
    return currentMemoryUsage;
}

long long Process::getMaxMemoryUsage(PROCESS_INFORMATION& processInfo, long long memoryLimit) {
    long long maxMemoryUsage = 0,
        currentMemoryUsage = 0;
    do {
        currentMemoryUsage = getCurrentMemoryUsage(processInfo.hProcess);
        if (currentMemoryUsage > maxMemoryUsage) {
            maxMemoryUsage = currentMemoryUsage;
        }
        if (memoryLimit != 0 && currentMemoryUsage > memoryLimit) {
            killProcess(processInfo);
        }
        Sleep(200);
    } while (getExitCode(processInfo.hProcess) == STILL_ACTIVE);
    //std::min
    return maxMemoryUsage;
}

DWORD Process::getExitCode(HANDLE& hProcess) {
    DWORD exitCode = 0;
    GetExitCodeProcess(hProcess, &exitCode);

    return exitCode;
}

bool Process::killProcess(PROCESS_INFORMATION& processInfo) {
#ifdef _DBG_
    std::cout << "Killing process" << std::endl;
#endif

    DWORD           processId = processInfo.dwProcessId;
    PROCESSENTRY32 processEntry = { 0 };
    processEntry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE handleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (Process32First(handleSnap, &processEntry)) {
        BOOL isContinue = TRUE;

        do {
            if (processEntry.th32ParentProcessID == processId) {
                HANDLE hChildProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processEntry.th32ProcessID);
                if (hChildProcess) {
                    TerminateProcess(hChildProcess, 1);
                    CloseHandle(hChildProcess);
                }
            }
            isContinue = Process32Next(handleSnap, &processEntry);
        } while (isContinue);

        HANDLE hBaseProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
        if (hBaseProcess) {
            TerminateProcess(hBaseProcess, 1);
            CloseHandle(hBaseProcess);
        }
    }

    if (getExitCode(processInfo.hProcess) == STILL_ACTIVE) {
        return false;
    }
    return true;
}

