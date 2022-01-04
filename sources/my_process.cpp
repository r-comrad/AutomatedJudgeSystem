//--------------------------------------------------------------------------------

#include "my_process.h"

#ifdef LINUS_LINUX
#include <unistd.h>
#endif

MyProcess::MyProcess
(
    std::string& aName,
    std::string& aParameters
) :
#ifdef BILL_WINDOWS
    mStartupInfo    ({ 0 })
#endif
{
#ifdef BILL_WINDOWS
    ZeroMemory(&mProcessInfo, sizeof(PROCESS_INFORMATION));
#endif
}

MyProcess::~MyProcess() {}

void
MyProcess::run(uint_16 aTimeLimit)
{
    WD_LOG("Runing simple process");

    ResumeThread(mProcessInfo.hThread);
    WaitForSingleObject(mProcessInfo.hProcess, aTimeLimit);

    if (getExitCode(mProcessInfo.hProcess) == STILL_ACTIVE)
    {
        killProcess(mProcessInfo);
    }

    closeHandles();

    WD_END_LOG;
}

std::pair<uint_64, uint_64>
MyProcess::runWithLimits
(
    uint_64 aTimeLimit,
    uint_64 aMemoryLimit
)
{
    WD_LOG("Runing process with time and memory evaluation");

    int reservedTime = 200;
    long long startTime = getMillisecondsNow();

    run(aTimeLimit);

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
MyProcess::create
(
    std::string& aName,
    std::string& aParameters
)
{
    aParameters = aName + aParameters;

    WD_LOG("Creating process name: " << aName);
    WD_LOG("Parameters: " << aParameters);
#ifdef BILL_WINDOWS
    char* name = (char*) aName.c_str();
    if (aName == "") name = NULL;

    char* cmd = (char*) (aParameters).c_str();
    if (aParameters == "") cmd = NULL;

    mFuture = std::async(std::launch::async, &MyProcess::getMaxMemoryUsage,
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
    ) == FALSE)
    {
        WD_ERROR(process.0, "Can't start process " + aName + 
            "\narguments are: " + aParameters);
    }
#endif
#ifdef LINUS_LINUX
int id = fork();
if (id == 0)
{
    int i = 0;
    int j = 0;
    //std::vector<const char*> param;
    char* param[10];
    while (true)
    {
        std::string s;
        for(;aParameters[i] != ' '; ++i) s.push_back(aParameters[i]);
        ++i;
        param[j] = (char*) s.c_str();
        ++j;
    }
    //char** param2 =
    //param.push_back(NULL);
    param[j] = NULL;
    execv((char*) aName.c_str(), param);
}
#endif
    WD_END_LOG;
}

long long 
MyProcess::getMillisecondsNow()
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
MyProcess::getCurrentMemoryUsage
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
MyProcess::getMaxMemoryUsage
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
MyProcess::getExitCode
(
    HANDLE& hProcess
) 
{
    DWORD exitCode = 0;
    GetExitCodeProcess(hProcess, &exitCode);
    return exitCode;
}

bool 
MyProcess::killProcess
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
                HANDLE hChildProcess = 
                    OpenProcess(PROCESS_ALL_ACCESS, FALSE,
                        processEntry.th32ProcessID);
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

//--------------------------------------------------------------------------------