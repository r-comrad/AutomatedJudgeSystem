#pragma GCC diagnostic ignored "-Wwrite-strings"
#define INT_MAX 2147483647

#include "core.h"


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

Core::Core()    :
    mStartupInfo({ 0 })
{
    //mProcessInfo = 0;
    ZeroMemory(&mProcessInfo, sizeof(PROCESS_INFORMATION));
}

void Core::IORedirection(std::wstring& aInputPath, std::wstring& aOutputPath)
{
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    mInputHandle = CreateFile(aInputPath.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        &sa,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    mOutputHandle = CreateFile(aOutputPath.c_str(),
        FILE_APPEND_DATA,
        FILE_SHARE_WRITE,
        &sa,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    PROCESS_INFORMATION pi;
    BOOL ret = FALSE;
    DWORD flags = 0;// CREATE_NO_WINDOW;

    ZeroMemory(&mStartupInfo, sizeof(STARTUPINFO));
    mStartupInfo.cb = sizeof(STARTUPINFO);
    mStartupInfo.dwFlags |= STARTF_USESTDHANDLES;
    mStartupInfo.hStdInput = mInputHandle;
    mStartupInfo.hStdError = mOutputHandle;
    mStartupInfo.hStdOutput = mOutputHandle;
}

long long foo(PROCESS_INFORMATION pi, long long i)
{
    return 10;
}

void Core::runProcess(std::wstring aName, std::wstring aInputFilePath, std::wstring aOutputFilePath) {
    IORedirection(aInputFilePath, aOutputFilePath);
    wchar_t* cmd = const_cast<wchar_t*>(aName.c_str());
    
    PROCESS_INFORMATION pi;
    auto feature = std::async(std::launch::async, &Core::getMaxMemoryUsage, this, std::ref(mProcessInfo), 1000000);

    if (CreateProcess(NULL, cmd,
        NULL, NULL, TRUE, CREATE_UNICODE_ENVIRONMENT | CREATE_SUSPENDED | CREATE_NO_WINDOW, NULL, NULL, &mStartupInfo, &mProcessInfo) == FALSE)
    {
        #ifdef DBG
        std::cout << "ERROR #2: can't start process" << std::endl;
        #endif // DEBUG      
    }

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
    memoryUsage = feature.get();

    std::cout << "time usage: " << endTime - startTime << std::endl;
    std::cout << "memory usage: " << memoryUsage << std::endl;

    CloseHandle(mInputHandle);
    CloseHandle(mOutputHandle);
}

long long Core::getMaxMemoryUsage(PROCESS_INFORMATION& processInfo, long long memoryLimit) {
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

long long Core::getCurrentMemoryUsage(HANDLE& hProcess) {
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

DWORD Core::getExitCode(HANDLE& hProcess) {
    DWORD exitCode = 0;
    GetExitCodeProcess(hProcess, &exitCode);

    return exitCode;
}

bool Core::killProcess(PROCESS_INFORMATION& processInfo) {
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

long long Core::getMillisecondsNow() {
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