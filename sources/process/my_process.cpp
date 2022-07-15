//--------------------------------------------------------------------------------

#include "process/my_process.hpp"

MyProcess::MyProcess
(
    const std::vector<char*>& aParameters,
    uint_64 aTimeLimit,
    uint_64 aMemoryLimit
) :
    mTimeLimit      (aTimeLimit),
    mMemoryLimit    (aMemoryLimit)
//#ifdef BILL_WINDOWS
//    , 
//    mStartupInfo    ({ 0 })
//#elif defined(LINUS_LINUX)
//#endif
{
#ifdef BILL_WINDOWS
    ZeroMemory(&mProcessInfo, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&mStartupInfo, sizeof(STARTUPINFOA));
#endif
}

//--------------------------------------------------------------------------------

MyProcess::~MyProcess() {}

//--------------------------------------------------------------------------------

bool
MyProcess::run()
{
    WRITE_LOG("Runing_simple_process");
#ifdef BILL_WINDOWS
    ResumeThread(mProcessInfo.hThread);
    WaitForSingleObject(mProcessInfo.hProcess, mTimeLimit);

    if (getExitCode(mProcessInfo.hProcess) == STILL_ACTIVE)
    {
        killProcess(mProcessInfo);
        return false;
    }

    //closeHandles();
#else
    //sint_32 status;
    //int status;
    //wait(&status);
    //kill(mChildPID, SIGCONT);
    wait(NULL);
    //wait4(mChildPID,NULL,0,NULL);
#endif

    return true;
}

//--------------------------------------------------------------------------------

void
MyProcess::setLimits(uint_64 aTimeLimit, uint_64 aMemoryLimit)
{
    if (aTimeLimit > MAX_TIME_LIMIT) aTimeLimit = MAX_TIME_LIMIT;
    if (aMemoryLimit > MAX_MEMORY_LIMIT) aMemoryLimit = MAX_MEMORY_LIMIT;

    mTimeLimit = aTimeLimit;
    mMemoryLimit = aMemoryLimit;
}

//--------------------------------------------------------------------------------

std::pair<uint_64, uint_64>
MyProcess::runWithLimits()
{
    START_LOG_BLOCK("Runing_process_with_time_and_memory_evaluation");

    uint_64 timeUsage = 0;
    uint_64 memoryUsage = 0;

#ifdef BILL_WINDOWS
    int reservedTime = 200;
    long long startTime = getCPUTime();

    if (!run()) 
        return { KILLING_PROCESS_TIME_VALUE, 
        KILLING_PROCESS_MEMORY_VALUE };

    long long endTime = getCPUTime();
    timeUsage = endTime - startTime;

    memoryUsage = mFuture.get();

#elif defined(LINUS_LINUX)
    rusage resourseUsage;
    int status;
    wait4(mChildPID, &status, 0, &resourseUsage);
    int gg = WIFEXITED(status);

    //wait(NULL);

    timeUsage += resourseUsage.ru_utime.tv_sec * 1000000L;
    timeUsage += resourseUsage.ru_utime.tv_usec;
    timeUsage += resourseUsage.ru_stime.tv_sec * 1000000L;
    timeUsage += resourseUsage.ru_stime.tv_usec;
#endif 

    WRITE_LOG("status:", status);
    WRITE_LOG("WIFEXITED:", WIFEXITED(status));
    WRITE_LOG("WEXITSTATUS:", WEXITSTATUS(status));
    WRITE_LOG("WIFSIGNALED:", WIFSIGNALED(status));
    WRITE_LOG("WTERMSIG:", WTERMSIG(status));
    WRITE_LOG("WIFSTOPPED:", WIFSTOPPED(status));
    WRITE_LOG("time_usage:", timeUsage);
    END_LOG_BLOCK("memory_usage:", memoryUsage);
    if (!WIFEXITED(status)) 
        return { KILLING_PROCESS_TIME_VALUE,
        KILLING_PROCESS_MEMORY_VALUE };
    return { timeUsage , memoryUsage };
}

//--------------------------------------------------------------------------------

void 
MyProcess::create(const std::vector<char*>& aParameters)
{
    #ifdef BILL_WINDOWS
    WRITE_LOG("Creating_process_name:", aParameters[0]);

    //TODO: in my_strinh.hpp

    char* name = newCharPtrCopy(aParameters[0]);
    if (name[0] == 0) name = NULL;

    int size = aParameters.size() + 1;
    for (int i = 0; i < aParameters.size() - 1; ++i)
    {
        size += strlen(aParameters[i]);
    }

    char* cmd = new char[size];
    cmd[0] = 0;
    for (int i = 0; i < aParameters.size() - 1; ++i)
    {
        strCopy(cmd, aParameters[i]);
        if (cmd[0] != 0) strCopy(cmd, " ");
    }
    if (cmd[0] == 0) cmd = NULL;
    
    // TODO: end

    mFuture = std::async(std::launch::async, &MyProcess::getMaxMemoryUsage,
        this, std::ref(mProcessInfo), 1000000);


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
        WRITE_ERROR("MyProcess", 10, "Can't_start_process", aParameters[0]);
    }
    delete name;
    delete cmd;
    #endif
}

//--------------------------------------------------------------------------------

#ifdef BILL_WINDOWS
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

//--------------------------------------------------------------------------------

long long 
MyProcess::getCurrentMemoryUsage(HANDLE& hProcess) 
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

//--------------------------------------------------------------------------------

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
    return maxMemoryUsage;
}

DWORD 
MyProcess::getExitCode(HANDLE& hProcess) 
{
    DWORD exitCode = 0;
    GetExitCodeProcess(hProcess, &exitCode);
    return exitCode;
}

//--------------------------------------------------------------------------------

bool 
MyProcess::killProcess(PROCESS_INFORMATION& processInfo) 
{
    START_LOG_BLOCK("Killing_process");

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

    END_LOG_BLOCK("Process killed?");

    return true;
}
#endif // BILL_WINDOWS

//--------------------------------------------------------------------------------