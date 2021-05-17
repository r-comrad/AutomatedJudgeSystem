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

void IORedirection(STARTUPINFO& aStartInfo, std::wstring& aInputPath, std::wstring& aOutputPath)
{
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    HANDLE inputHandle = CreateFile(aInputPath.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        &sa,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    HANDLE outputHandle = CreateFile(aOutputPath.c_str(),
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

DWORD runProcess(PROCESS_INFORMATION& processInfo, long long timeLimit,
    long long memoryLimit, long long& timeUsage, long long& memoryUsage) {
    int  reservedTime = 200;
    auto feature = std::async(std::launch::async, getMaxMemoryUsage, std::ref(processInfo), memoryLimit);

    ResumeThread(processInfo.hThread);
    long long startTime = getMillisecondsNow();
    WaitForSingleObject(processInfo.hProcess, timeLimit + reservedTime);
    long long endTime = getMillisecondsNow();
    timeUsage = endTime - startTime;

    if (getExitCode(processInfo.hProcess) == STILL_ACTIVE) {
        killProcess(processInfo);
    }
    memoryUsage = feature.get();

    return getExitCode(processInfo.hProcess);
    return 0;
}

void myJudger(std::wstring aName, std::wstring aInputFilePath, std::wstring aOutputFilePath)
{
    //LPCWSTR name = aName.c_str();

    long long timeUsage = 1000;
    long long reservedTime = 200;
    long long memoryLimit = 200;

    //if (!setupIoRedirection(inputFilePath, outputFilePath, hInput, hOutput))
    //{
    //    std::cout << "ERROR #1: can't redirection paths" << std::endl;
    //}

    PROCESS_INFORMATION processInfo;
    ZeroMemory(&processInfo, sizeof(PROCESS_INFORMATION));

    STARTUPINFOW startupInfo = { 0 };
    IORedirection(startupInfo, aInputFilePath, aOutputFilePath);

    auto feature = std::async(std::launch::async, getMaxMemoryUsage, std::ref(processInfo), memoryLimit);

    TCHAR cmd[] = TEXT("U:\\_Public\\Programs\\a.exe");
    //TCHAR cmd[] = TEXT("U:\\_Public\\Programs\\Sleepy.exe");
    //TCHAR cmd[] = TEXT("U:\\_Public\\Programs\\summa.exe");
    if (CreateProcess(NULL, cmd,
        NULL, NULL, TRUE, CREATE_UNICODE_ENVIRONMENT | CREATE_SUSPENDED | CREATE_NO_WINDOW, NULL, NULL, &startupInfo, &processInfo) == FALSE)
    {
        std::cout << "ERROR #2: can't start process" << std::endl;
    }
    long long startTime = getMillisecondsNow();
    /*   if (!CreateProcess(name, NULL,
           NULL, NULL, FALSE, CREATE_UNICODE_ENVIRONMENT | CREATE_SUSPENDED | CREATE_NO_WINDOW, NULL, NULL, &startupInfo, &pi) == TRUE)*/
    ResumeThread(processInfo.hThread);
    DWORD waitResult = WaitForSingleObject(processInfo.hProcess, INFINITE);
    if (waitResult == WAIT_FAILED)
    {
        std::cout << "ERROR #3: WAIT_FAILED" << std::endl;
    }
    if (waitResult == WAIT_OBJECT_0)
    {
        std::cout << "Code complite\n" << std::endl;
    }
    if (waitResult == WAIT_TIMEOUT)
    {
        std::cout << "Time out\n" << std::endl;
    }
    if (getExitCode(processInfo.hProcess) == STILL_ACTIVE) {
        std::cout << "Alive" << std::endl;
        //killProcess(processInfo);
    }
    long long endTime = getMillisecondsNow();
    long long memoryUsage = feature.get();

    //CloseHandle(hInput);
    //CloseHandle(hOutput);

    std::cout << "time usage: " << endTime - startTime << std::endl;
    std::cout << "memory usage: " << memoryUsage << std::endl;
}

void myJudger2(std::wstring aName, std::wstring aInputFilePath, std::wstring aOutputFilePath)
{
    long long timeUsage = 1000;
    long long reservedTime = 200;
    long long memoryLimit = 200;

    CHandle Job(CreateJobObject(nullptr, nullptr));
    if (!Job) {
        std::cout << "ERROR #x: CreateJobObject, error " << GetLastError() << std::endl;
        return;
    }


    CHandle IOPort(CreateIoCompletionPort(INVALID_HANDLE_VALUE,
        nullptr, 0, 1));
    if (!IOPort) {
        std::cout << "ERROR #x: CreateIoCompletionPort, error " << GetLastError() << std::endl;
        return;
    }


    JOBOBJECT_ASSOCIATE_COMPLETION_PORT Port;
    Port.CompletionKey = Job;
    Port.CompletionPort = IOPort;
    if (!SetInformationJobObject(Job,
        JobObjectAssociateCompletionPortInformation,
        &Port, sizeof(Port))) {
        std::cout << "ERROR #x: SetInformation, error " << GetLastError() << std::endl;
        return;
    }
    
    PROCESS_INFORMATION processInfo;
    ZeroMemory(&processInfo, sizeof(PROCESS_INFORMATION));

    STARTUPINFOW startupInfo = { 0 };
    IORedirection(startupInfo, aInputFilePath, aOutputFilePath);

    TCHAR cmd[] = TEXT("U:\\_Public\\Programs\\a.exe");
    //TCHAR cmd[] = TEXT("U:\\_Public\\Programs\\Sleepy.exe");
    //TCHAR cmd[] = TEXT("U:\\_Public\\Programs\\summa.exe");

    auto feature = std::async(std::launch::async, getMaxMemoryUsage, std::ref(processInfo), memoryLimit);

    if (CreateProcess(NULL, cmd,
        NULL, NULL, TRUE, CREATE_UNICODE_ENVIRONMENT | CREATE_SUSPENDED | CREATE_NO_WINDOW, NULL, NULL, &startupInfo, &processInfo) == FALSE)
    {
        std::cout << "ERROR #2: can't start process" << std::endl;
    }
    long long startTime = getMillisecondsNow();
    ResumeThread(processInfo.hThread);
    DWORD waitResult = WaitForSingleObject(processInfo.hProcess, INFINITE);
    if (waitResult == WAIT_FAILED)
    {
        std::cout << "ERROR #3: WAIT_FAILED" << std::endl;
    }
    if (waitResult == WAIT_OBJECT_0)
    {
        std::cout << "Code complite\n" << std::endl;
    }
    if (waitResult == WAIT_TIMEOUT)
    {
        std::cout << "Time out\n" << std::endl;
    }
    if (getExitCode(processInfo.hProcess) == STILL_ACTIVE) {
        std::cout << "Alive" << std::endl;
    }


    if (!AssignProcessToJobObject(Job,
        processInfo.hProcess)) {
        std::cout << "ERROR #x: Assign, error " << GetLastError() << std::endl;
        return;
    }


    ResumeThread(processInfo.hThread);
    CloseHandle(processInfo.hThread);
    CloseHandle(processInfo.hProcess);


    DWORD CompletionCode;
    ULONG_PTR CompletionKey;
    LPOVERLAPPED Overlapped;


    while (GetQueuedCompletionStatus(IOPort, &CompletionCode,
        &CompletionKey, &Overlapped, INFINITE) &&
        !((HANDLE)CompletionKey == Job &&
            CompletionCode == JOB_OBJECT_MSG_ACTIVE_PROCESS_ZERO)) {

    }


    long long endTime = getMillisecondsNow();
    long long memoryUsage = feature.get();

    std::cout << "time usage: " << endTime - startTime << std::endl;
    std::cout << "memory usage: " << memoryUsage << std::endl;
}

int __cdecl myJudger3(std::wstring aName, std::wstring aInputFilePath, std::wstring aOutputFilePath)
{
    CHandle Job(CreateJobObject(nullptr, nullptr));
    if (!Job) {
        wprintf(L"CreateJobObject, error % d\n", GetLastError());
        return 0;
    }


    CHandle IOPort(CreateIoCompletionPort(INVALID_HANDLE_VALUE,
        nullptr, 0, 1));
    if (!IOPort) {
        wprintf(L"CreateIoCompletionPort, error % d\n",
            GetLastError());
        return 0;
    }


    JOBOBJECT_ASSOCIATE_COMPLETION_PORT Port;
    Port.CompletionKey = Job;
    Port.CompletionPort = IOPort;
    if (!SetInformationJobObject(Job,
        JobObjectAssociateCompletionPortInformation,
        &Port, sizeof(Port))) {
        wprintf(L"SetInformation, error % d\n", GetLastError());
        return 0;
    }


    PROCESS_INFORMATION ProcessInformation;
    STARTUPINFO StartupInfo = { sizeof(StartupInfo) };
    IORedirection(StartupInfo, aInputFilePath, aOutputFilePath);
    //PWSTR CommandLine = aName.c_str();
    TCHAR cmd[] = TEXT("U:\\_Public\\Programs\\a.exe");

    /*
    
    if (CreateProcess(NULL, cmd,
        NULL, NULL, TRUE, CREATE_UNICODE_ENVIRONMENT | CREATE_SUSPENDED | CREATE_NO_WINDOW, NULL, NULL, &startupInfo, &processInfo) == FALSE)
    {
    */
    long long memoryLimit = 2e6;
    auto feature = std::async(std::launch::async, getMaxMemoryUsage, std::ref(ProcessInformation), memoryLimit);

    if (!CreateProcess(nullptr, cmd, nullptr, nullptr,
        TRUE, CREATE_UNICODE_ENVIRONMENT | CREATE_SUSPENDED | CREATE_NO_WINDOW, nullptr, nullptr,
        &StartupInfo, &ProcessInformation)) {
        wprintf(L"CreateProcess, error % d\n", GetLastError());
        return 0;
    }


    if (!AssignProcessToJobObject(Job,
        ProcessInformation.hProcess)) {
        wprintf(L"Assign, error % d\n", GetLastError());
        return 0;
    }

    long long startTime = getMillisecondsNow();
    ResumeThread(ProcessInformation.hThread);


    long long memoryUsage = feature.get();

    CloseHandle(ProcessInformation.hThread);
    CloseHandle(ProcessInformation.hProcess);

    DWORD CompletionCode;
    ULONG_PTR CompletionKey;
    LPOVERLAPPED Overlapped;

    while (GetQueuedCompletionStatus(IOPort, &CompletionCode,
        &CompletionKey, &Overlapped, INFINITE) &&
        !((HANDLE)CompletionKey == Job &&
            CompletionCode == JOB_OBJECT_MSG_ACTIVE_PROCESS_ZERO)) {
        wprintf(L"Still waiting…\n");
    }
    long long endTime = getMillisecondsNow();

    std::cout << "\n\ntime usage: " << endTime - startTime << std::endl;
    std::cout << "\n\nmemory usage: " << memoryUsage << std::endl;









    wprintf(L"All done\n");


    return 0;
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
    std::wstring programPath =  L"U:\\_Public\\Programs\\a.exe";
    std::wstring inputPath =    L"U:\\_Public\\Programs\\input";
    std::wstring outputPath =   L"U:\\_Public\\Programs\\output";
    myJudger3(programPath, inputPath, outputPath);

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