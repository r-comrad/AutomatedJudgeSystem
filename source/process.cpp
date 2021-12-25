#pragma GCC diagnostic ignored "-Wwrite-strings"
#define INT_MAX 2147483647

#include "process.h"


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
    mStartupInfo    ({ 0 }),
    mThisSTDIN      (0),
    mThisSTDOUT     (0),
    mChildSTDIN     (0),
    mChildSTDOUT    (0)
{
    ZeroMemory(&mProcessInfo, sizeof(PROCESS_INFORMATION));
}

void 
Process::IORedirection
(
    IOType          aType,
    std::string    aInputPath,
    std::string    aOutputPath
)
{
    SECURITY_ATTRIBUTES securatyAttributes;
    securatyAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
    securatyAttributes.lpSecurityDescriptor = NULL;
    securatyAttributes.bInheritHandle = true;

    if (aType == Process::IOType::NONE) {}
    else if (aType == Process::IOType::FILES) 
    {
        WD_LOG("Rederecting input to: " <<aInputPath);
        WD_LOG("Rederecting output to: " << aOutputPath);

        if (aInputPath != "") mChildSTDIN = CreateFileA(aInputPath.c_str(),
            GENERIC_READ,
            FILE_SHARE_READ,
            &securatyAttributes,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

        if (aOutputPath != "") mChildSTDOUT = CreateFileA(aOutputPath.c_str(),
            FILE_WRITE_DATA,
            FILE_SHARE_WRITE,
            &securatyAttributes,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
    }
    else if (aType == Process::IOType::PIPES) 
    {
        WD_LOG("Rederecting input & output to pipe");
        if (!CreatePipe(&mChildSTDIN, &mThisSTDOUT, &securatyAttributes, 0))
        {
            WD_ERROR(process.pipe.0, "Can't create pipe ");
        }

        if (!CreatePipe(&mThisSTDIN, &mChildSTDOUT, &securatyAttributes, 0))
        {
            WD_ERROR(process.pipe.1, "Can't create pipe ");
        }
    }
    //else if (aType == Process::IOType::MIXED) {}

    //GetStartupInfo(&mStartupInfo);
    ZeroMemory(&mStartupInfo, sizeof(STARTUPINFO));
    mStartupInfo.cb = sizeof(STARTUPINFO);
    mStartupInfo.dwFlags |= STARTF_USESTDHANDLES;

    mStartupInfo.hStdInput = mChildSTDIN;
    mStartupInfo.hStdError = mChildSTDOUT;
    mStartupInfo.hStdOutput = mChildSTDOUT;

    WD_END_LOG;
}

void
Process::readPipe(std::string& result)
{
 //   unsigned long exit;
  //  GetExitCodeProcess(mProcessInfo.hProcess, &exit); //пока дочерний процесс                                      // не закрыт
   // if (exit != STILL_ACTIVE) return "";

    WD_LOG("Reading from pipe");
    char buf[1024];
    memset(buf, 0, sizeof(buf));

    unsigned long bread = 0;   //кол-во прочитанных байт
    unsigned long avail = 0;   //кол-во доступных байт

    while (bread == 0 && avail == 0)
    {
        PeekNamedPipe(mThisSTDIN, buf, 1023, &bread, &avail, NULL);
    }

    memset(buf, 0, sizeof(buf));
    bread = 1024;
    result.clear();
    while (bread >= 1023)
    {
        memset(buf, 0, sizeof(buf));
        ReadFile(mThisSTDIN, buf, 1023, &bread, NULL);
        result += std::string(buf);
    }

    WD_END_LOG;
}

void
Process::writePipe(std::string aMessage, PypeType aType)
{
    WD_LOG("Writing from pipe");
    unsigned long bread;
    WriteFile(mThisSTDOUT, aMessage.c_str(), aMessage.size() + ((aType == ZERO) ? 1 : 0), &bread, NULL);
    WD_LOG("write " + std::to_string(bread) + " bites\n");
    WD_END_LOG;
}

void 
Process::create
(
    std::string aName,
    std::string aParameters
)
{
    WD_LOG("Creating process name: " << aName);
    WD_LOG("Parameters: " << aParameters);

    char* name = (char*) aName.c_str();
    if (aName == "") name = NULL;

    char* cmd = (char*) (aParameters).c_str();
    if (aParameters == "") cmd = NULL;

    mFuture = std::async(std::launch::async, &Process::getMaxMemoryUsage,
        this, std::ref(mProcessInfo), 1000000);
    //TODO: Memory limit

    if (CreateProcessA(
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
    ) == FALSE) WD_ERROR(process.0, "Can't start process " + aName + "\narguments are: " + aParameters);


    WD_END_LOG;
}

void 
Process::run()
{
    WD_LOG("Runing simple process");

    ResumeThread(mProcessInfo.hThread);
    WaitForSingleObject(mProcessInfo.hProcess, INFINITE);
    //CloseHandle(mChildSTDIN);
    //CloseHandle(mChildSTDOUT);

    WD_END_LOG;
}

std::pair<uint_64, uint_64> 
Process::runWithLimits
(
    uint_64 aTimeLimit, 
    uint_64 aMemoryLimit
)
{
    WD_LOG("Runing process with time and memory evaluation");

    int  reservedTime = 200;
    ResumeThread(mProcessInfo.hThread);
    long long startTime = getMillisecondsNow();
    WaitForSingleObject(mProcessInfo.hProcess, INFINITE);

    /*
    GetExitCodeProcess(pi.hProcess,&exit); //пока дочерний процесс
                                           // не закрыт
    if (exit != STILL_ACTIVE)
      break;
    */

    if (getExitCode(mProcessInfo.hProcess) == STILL_ACTIVE) //TODO: get 
    {
        killProcess(mProcessInfo); //TODO: kill
    }

    CloseHandle(mChildSTDIN);
    CloseHandle(mChildSTDOUT);

    long long endTime = getMillisecondsNow();
    uint_64 timeUsage = endTime - startTime;

    uint_64 memoryUsage = 0;
    memoryUsage = mFuture.get();

    WD_LOG("time usage: " << timeUsage);
    WD_LOG("memory usage: " << memoryUsage);
    WD_END_LOG;

    return { timeUsage , memoryUsage };
}

void
Process::closeIO()
{
    CloseHandle(mThisSTDIN);
    CloseHandle(mThisSTDOUT);
    CloseHandle(mChildSTDIN);
    CloseHandle(mChildSTDOUT);
}

long long 
Process::getMillisecondsNow() 
{
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

long long 
Process::getCurrentMemoryUsage
(
    HANDLE& hProcess
) 
{
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

long long 
Process::getMaxMemoryUsage
(
    PROCESS_INFORMATION& processInfo, 
    long long memoryLimit
) 
{
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

DWORD 
Process::getExitCode
(
    HANDLE& hProcess
) 
{
    DWORD exitCode = 0;
    GetExitCodeProcess(hProcess, &exitCode);

    return exitCode;
}

bool 
Process::killProcess
(
    PROCESS_INFORMATION& processInfo
) 
{
    WD_LOG("Killing process");

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

    WD_LOG("Process killed?");
    WD_END_LOG;

    return true;
}

