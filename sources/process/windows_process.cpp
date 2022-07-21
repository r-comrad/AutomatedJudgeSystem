//--------------------------------------------------------------------------------

#include "windows_process.hpp"

proc::WindowsProcess::WindowsProcess()  noexcept
{
    ZeroMemory(&mProcessInfo, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&mStartupInfo, sizeof(STARTUPINFOA));
    setLimits(1e18, 1e18);

    mProcessArgs.switchToCharArray();
}

proc::WindowsProcess::WindowsProcess(const WindowsProcess& other)  noexcept
{
    mProcessName = other.mProcessName;
    mProcessArgs = other.mProcessArgs.getCopy(dom::String::StrType::CharArray);
}

proc::WindowsProcess&
proc::WindowsProcess::operator=(const WindowsProcess& other) noexcept
{
    mProcessName = other.mProcessName;
    mProcessArgs = other.mProcessArgs.getCopy(dom::String::StrType::CharArray);
    return *this;
}

// proc::WindowsProcess::~WindowsProcess()  noexcept
// {
//     //if (mFuture != nullptr) delete mFuture;
// }

//--------------------------------------------------------------------------------

bool
proc::WindowsProcess::run() noexcept
{
    bool result = true;
    WRITE_LOG("Runing_simple_windows_process");

    // if (getExitCode(mProcessInfo.hProcess) == STILL_ACTIVE)
    // {
    // }
    ResumeThread(mProcessInfo.hThread);
    WaitForSingleObject(mProcessInfo.hProcess, mTimeLimit);

    if (getExitCode(mProcessInfo.hProcess) == STILL_ACTIVE)
    {
        killProcess(mProcessInfo);
        result = false;
    }

    return result;
}

//--------------------------------------------------------------------------------

std::optional<dom::Pair<uint_64>> 
proc::WindowsProcess::runWithLimits() noexcept
{
    START_LOG_BLOCK("Runing_windows_process_with_time_and_memory_evaluation");

    std::optional<dom::Pair<uint_64>> result = {}; 

    uint_64 timeUsage = 0;
    uint_64 memoryUsage = 0;

    int reservedTime = 200;
    long long startTime = getCPUTime();

    if (run()) 
    {
        long long endTime = getCPUTime();
        timeUsage = endTime - startTime;

    //    memoryUsage = mFuture->get();

        WRITE_LOG("time_usage:", timeUsage);
        END_LOG_BLOCK("memory_usage:", memoryUsage);

        result = { timeUsage , memoryUsage };
    }

    return result;
}

//--------------------------------------------------------------------------------

void 
proc::WindowsProcess::setComand(const dom::StringTable& aParameters) noexcept
{
    mProcessName = dom::CharArray(aParameters[0]);
    
    for(const auto& str : aParameters)
    {
        mProcessArgs += str;
        mProcessArgs += " ";
    }
    mProcessArgs.merge();
}

//--------------------------------------------------------------------------------

void 
proc::WindowsProcess::create() noexcept
{
    START_LOG_BLOCK("Creating_windows_process_with_name:", mProcessName.get());
    WRITE_LOG("args:", mProcessArgs.getFront<char*>());

    //mFuture = new std::future<long long>;
   //  *mFuture = (std::async(std::launch::async, &proc::WindowsProcess::getMaxMemoryUsage,
    //    this, std::ref(mProcessInfo), 1000000));

    //TODO:     if (name[0] == 0) name = NULL;
    if (CreateProcessA(
        //TODO: without string
        mProcessName.get(),
        mProcessArgs.getFront<char*>(),
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
        WRITE_ERROR("Process", "create", 10, "Can't_start_process", mProcessName.get());
    }
    END_LOG_BLOCK();
}

//--------------------------------------------------------------------------------

long long 
proc::WindowsProcess::getMillisecondsNow() noexcept
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
proc::WindowsProcess::getCurrentMemoryUsage(HANDLE& hProcess) noexcept
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
proc::WindowsProcess::getMaxMemoryUsage
(
    PROCESS_INFORMATION& processInfo, 
    long long memoryLimit
) noexcept
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
proc::WindowsProcess::getExitCode(HANDLE& hProcess) noexcept
{
    DWORD exitCode = 0;
    GetExitCodeProcess(hProcess, &exitCode);
    return exitCode;
}

//--------------------------------------------------------------------------------

bool 
proc::WindowsProcess::killProcess(PROCESS_INFORMATION& processInfo) noexcept
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

//--------------------------------------------------------------------------------