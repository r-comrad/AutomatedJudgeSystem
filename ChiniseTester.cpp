#pragma GCC diagnostic ignored "-Wwrite-strings"
#define INT_MAX 2147483647

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

/**
 * Function Prototypes.
 */
bool setupIoRedirection(std::wstring, std::wstring, HANDLE&, HANDLE&);
void setupStartupInfo(STARTUPINFOW&, HANDLE&, HANDLE&);
bool createProcess(const std::wstring&, const std::wstring&, const std::wstring&, HANDLE&, LPVOID, STARTUPINFOW&, PROCESS_INFORMATION&);
DWORD runProcess(PROCESS_INFORMATION&, long long, long long, long long&, long long&);
long long getMaxMemoryUsage(PROCESS_INFORMATION&, long long);
long long getCurrentMemoryUsage(HANDLE&);
long long getMillisecondsNow();
bool killProcess(PROCESS_INFORMATION&);
DWORD getExitCode(HANDLE&);
std::string getErrorMessage(const std::string&);

long long getMaxMemoryUsage(PROCESS_INFORMATION& processInfo, long long memoryLimit) {
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

long long getCurrentMemoryUsage(HANDLE& hProcess) {
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

long long getMillisecondsNow() {
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

bool killProcess(PROCESS_INFORMATION& processInfo) {
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

DWORD getExitCode(HANDLE& hProcess) {
    DWORD exitCode = 0;
    GetExitCodeProcess(hProcess, &exitCode);

    return exitCode;
}

std::string getErrorMessage(const std::string& apiName) {
    LPVOID lpvMessageBuffer;

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&lpvMessageBuffer, 0, NULL);

    std::stringstream stringStream;
    std::string errorMessage((LPSTR)lpvMessageBuffer);

    stringStream << "API:     " << apiName << std::endl
        << "Code:    " << GetLastError() << std::endl
        << "Message: " << errorMessage << std::endl;
    LocalFree(lpvMessageBuffer);

    return stringStream.str();
    return "";
}

HANDLE inputHandle;
HANDLE outputHandle;

void IORedirection(STARTUPINFO& aStartInfo, std::wstring& aInputPath, std::wstring& aOutputPath)
{
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    inputHandle = CreateFile(aInputPath.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        &sa,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    outputHandle = CreateFile(aOutputPath.c_str(),
        FILE_APPEND_DATA,
        FILE_SHARE_WRITE,
        &sa,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    PROCESS_INFORMATION pi;
    BOOL ret = FALSE;
    DWORD flags = 0;// CREATE_NO_WINDOW;

    ZeroMemory(&aStartInfo, sizeof(STARTUPINFO));
    aStartInfo.cb = sizeof(STARTUPINFO);
    aStartInfo.dwFlags |= STARTF_USESTDHANDLES;
    aStartInfo.hStdInput = inputHandle;
    aStartInfo.hStdError = outputHandle;
    aStartInfo.hStdOutput = outputHandle;
}

DWORD runProcess(std::wstring aName, std::wstring aInputFilePath, std::wstring aOutputFilePath) {
    PROCESS_INFORMATION processInfo;
    ZeroMemory(&processInfo, sizeof(PROCESS_INFORMATION));

    STARTUPINFOW startupInfo = { 0 };
    IORedirection(startupInfo, aInputFilePath, aOutputFilePath);
    wchar_t* cmd = const_cast<wchar_t*>(aName.c_str());

    auto feature = std::async(std::launch::async, getMaxMemoryUsage, std::ref(processInfo), 1000000);

    if (CreateProcess(NULL, cmd,
        //   NULL, NULL, TRUE, CREATE_UNICODE_ENVIRONMENT | CREATE_SUSPENDED | CREATE_NO_WINDOW, NULL, NULL, &startupInfo, &processInfo) == FALSE)
        NULL, NULL, TRUE, CREATE_UNICODE_ENVIRONMENT | CREATE_SUSPENDED | CREATE_NO_WINDOW, NULL, NULL, &startupInfo, &processInfo) == FALSE)
    {
        std::cout << "ERROR #2: can't start process" << std::endl;
    }

    int  reservedTime = 200;


    ResumeThread(processInfo.hThread);
    long long startTime = getMillisecondsNow();
    //WaitForSingleObject(processInfo.hProcess, 1 + reservedTime);
    WaitForSingleObject(processInfo.hProcess, INFINITE);
    if (getExitCode(processInfo.hProcess) == STILL_ACTIVE)
    {
        killProcess(processInfo);
    }
    //LPDWORD res = 0;

    //while (GetExitCodeProcess(processInfo.hProcess, res) == STILL_ACTIVE)
    //{
    //    std::cout << "working\n";
    //}


    long long endTime = getMillisecondsNow();
    uint_64 timeUsage = endTime - startTime;

    //if (getExitCode(processInfo.hProcess) == STILL_ACTIVE) {
    //    killProcess(processInfo);
    //}
    uint_64 memoryUsage = 0;
    memoryUsage = feature.get();

    std::cout << "time usage: " << endTime - startTime << std::endl;
    std::cout << "memory usage: " << memoryUsage << std::endl;

    CloseHandle(inputHandle);
    CloseHandle(outputHandle);
    return getExitCode(processInfo.hProcess);
}

#include <vector>
#include <iostream>
#include <string>
int main()
{
    //long long ll = GetProcessTimes();
    //long long ll = GetProcessMemoryInfo ();


    //std::wstring programPath = L"D:\\projects\\VS 2019\\ChiniseTester\\Resources\\YesInput.exe";
    //std::wstring inputPath = L"D:\\projects\\VS 2019\\ChiniseTester\\Resources\\input.txt";
    //std::wstring outputPath = L"D:\\projects\\VS 2019\\ChiniseTester\\Resources\\output.txt";
    std::wstring path = L"D:\\projects\\VS 2019\\ChiniseTester\\Resources\\";
    std::wstring programPath = path + L"NeoTest2.exe";
    std::wstring inputPath = path + L"input";
    std::wstring outputPath = path + L"output.a";
    //myJudger(programPath, inputPath, outputPath);
    runProcess(programPath, inputPath, outputPath);
    
    //foo("U:\\_Private\\Tester\\Programs\\b.exe", NULL,
    //    NULL, NULL, NULL, 1000,
    //    1000);
    //foo("c:\\c.exe", "",
    //    "", "", "", 1000,
    //    1000);

    //STARTUPINFO cif;
    //ZeroMemory(&cif, sizeof(STARTUPINFO));
    //PROCESS_INFORMATION pi;
    //if (CreateProcess(L"c:\\c.exe", NULL,
    //    NULL, NULL, FALSE, NULL, NULL, NULL, &cif, &pi) == TRUE)
    //{
    //    //cout << "process" << endl;
    //    //cout << "handle " << pi.hProcess << endl;
    //    //Sleep(1000);                // подождать
    //    //TerminateProcess(pi.hProcess, NO_ERROR);    // убрать процесс
    //}

    //long long                timeUsage = 1000;
    //long long                memoryUsage = 0;
    //int  reservedTime = 0;
    //WaitForSingleObject(pi.hProcess, timeUsage + reservedTime);
    //TerminateProcess(pi.hProcess, NO_ERROR);

    //STARTUPINFO cif;
    //ZeroMemory(&cif, sizeof(STARTUPINFO));
    //PROCESS_INFORMATION pi;
    //if (CreateProcess(L"c:\\windows\\notepad.exe", NULL,
    //    NULL, NULL, FALSE, NULL, NULL, NULL, &cif, &pi) == TRUE)
    //{
    //    std::cout << "process" << std::endl;
    //    std::cout << "handle " << pi.hProcess << std::endl;
    //    Sleep(10000);                // подождать
    //    TerminateProcess(pi.hProcess, NO_ERROR);    // убрать процесс
    //}

    return 0;
}