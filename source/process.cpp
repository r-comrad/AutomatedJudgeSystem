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
    mStartupInfo({ 0 })
{
    //mProcessInfo = 0;
    ZeroMemory(&mProcessInfo, sizeof(PROCESS_INFORMATION));
}

void 
Process::IORedirection
(
    std::wstring aInputPath,
    std::wstring aOutputPath
)
{
    WD_LOG("Rederecting input to: " << makeGoodString(aInputPath));
    WD_LOG("Rederecting output to: " << makeGoodString(aOutputPath));

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
    else
    {
        if (!CreatePipe(&newstdin, &write_stdin, &sa, 0))
        {
            ErrorMessage("CreatePipe");
            _getch();
            return;
        }
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
    else
    {
        if (!CreatePipe(&read_stdout, &newstdout, &sa, 0))
        {
            ErrorMessage("CreatePipe");
            _getch();
            CloseHandle(newstdin);
            CloseHandle(write_stdin);
            return;
        }
    }

    //BOOL ret = FALSE;
    //DWORD flags = 0;// CREATE_NO_WINDOW;

    ZeroMemory(&mStartupInfo, sizeof(STARTUPINFO));
    mStartupInfo.cb = sizeof(STARTUPINFO);
    mStartupInfo.dwFlags |= STARTF_USESTDHANDLES;
    if (aInputPath != L"")  mStartupInfo.hStdInput = mInputHandle;
    if (aOutputPath != L"") mStartupInfo.hStdError = mOutputHandle;
    if (aOutputPath != L"") mStartupInfo.hStdOutput = mOutputHandle;

    WD_END_LOG;
}

std::string
Process::readPipe()
{
    if (exit != STILL_ACTIVE)
        break;

    PeekNamedPipe(read_stdout, buf, 1023, &bread, &avail, NULL);

    //Проверяем, есть ли данные для чтения в stdout

    if (bread != 0)
    {
        bzero(buf);
        if (avail > 1023)
        {
            while (bread >= 1023)
            {
                ReadFile(read_stdout, buf, 1023, &bread, NULL);  //читаем из
                                                             // пайпа stdout
                printf("%s", buf);
                bzero(buf);
            }
        }
    }
}

void
Process::writePipe()
{
    if (exit != STILL_ACTIVE)
        break;

    PeekNamedPipe(read_stdout, buf, 1023, &bread, &avail, NULL);

    //Проверяем, есть ли данные для чтения в stdout

    if (bread != 0)
    {
        bzero(buf);
        if (avail > 1023)
        {
            while (bread >= 1023)
            {
                ReadFile(read_stdout, buf, 1023, &bread, NULL);  //читаем из
                                                                // пайпа stdout
                printf("%s", buf);
                bzero(buf);
            }
        }
    }
}

void
Process::writePipe()
{
    bzero(buf);
    *buf = (char)_getch();

    //printf("%c",*buf);

    WriteFile(write_stdin, buf, 1, &bread, NULL); //отправляем это
                                              // в stdin

    if (*buf == '\r') {
        *buf = '\n';
        printf("%c", *buf);
        WriteFile(write_stdin, buf, 1, &bread, NULL); //формирум конец
                                                  //строки, если нужно

    }
}

void 
Process::create
(
    std::wstring aName,
    std::wstring aParameters
)
{
    WD_LOG("Creating process name: " << makeGoodString(aName));
    WD_LOG("Parameters: " << makeGoodString(aParameters));

    wchar_t* name = const_cast<wchar_t*>(aName.c_str());
    if (aName == L"") name = NULL;

    wchar_t* cmd = const_cast<wchar_t*>(aParameters.c_str());
    if (aParameters == L"") cmd = NULL;

    mFuture = std::async(std::launch::async, &Process::getMaxMemoryUsage, 
        this, std::ref(mProcessInfo), 1000000);
    //TODO: Memory limit

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
    ) == FALSE) WD_ERROR(process.0, "Can't start process " + makeGoodString(aName) + "\narguments are: " + makeGoodString(aParameters));

    WD_END_LOG;
}

void 
Process::run()
{
    WD_LOG("Runing simple process");

    ResumeThread(mProcessInfo.hThread);
    WaitForSingleObject(mProcessInfo.hProcess, INFINITE);
    CloseHandle(mInputHandle);
    CloseHandle(mOutputHandle);

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

    if (getExitCode(mProcessInfo.hProcess) == STILL_ACTIVE) //TODO: get 
    {
        killProcess(mProcessInfo); //TODO: kill
    }

    CloseHandle(mInputHandle);
    CloseHandle(mOutputHandle);

    long long endTime = getMillisecondsNow();
    uint_64 timeUsage = endTime - startTime;

    uint_64 memoryUsage = 0;
    memoryUsage = mFuture.get();

    WD_LOG("time usage: " << timeUsage);
    WD_LOG("memory usage: " << memoryUsage);
    WD_END_LOG;

    return { timeUsage , memoryUsage };
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

