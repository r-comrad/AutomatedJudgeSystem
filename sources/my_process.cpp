//--------------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include "my_process.h"
#include "getCPUTime.h"

#ifdef LINUS_LINUX
#include <unistd.h>
#endif

//MyProcess::MyProcess()
//#ifdef BILL_WINDOWS
//:
//    mStartupInfo    ({ 0 })
//#endif
//{
//#ifdef BILL_WINDOWS
//    ZeroMemory(&mProcessInfo, sizeof(PROCESS_INFORMATION));
//#endif
//}



MyProcess::MyProcess(const std::vector<char*>& aParameters, uint_64 aTimeLimit, uint_64 aMemoryLimit)
#ifdef BILL_WINDOWS
:
    mStartupInfo    ({ 0 })
#elif defined(LINUS_LINUX)
   // mIsPaused       (true)
#endif
{
    //bool MyProcess::mIsPaused = false;
#ifdef BILL_WINDOWS
    ZeroMemory(&mProcessInfo, sizeof(PROCESS_INFORMATION));
#endif
    IORedirection   ();
    setLimits(aTimeLimit, aMemoryLimit);
    create          (aParameters);

}

MyProcess::~MyProcess() {}

bool
MyProcess::run(uint_16 aTimeLimit)
{
    WD_LOG("Runing simple process");
#ifdef BILL_WINDOWS
    ResumeThread(mProcessInfo.hThread);
    WaitForSingleObject(mProcessInfo.hProcess, aTimeLimit);

    if (getExitCode(mProcessInfo.hProcess) == STILL_ACTIVE)
    {
        killProcess(mProcessInfo);
        return false;
    }

    closeHandles();
#else
    //sint_32 status;
    //int status;
    //wait(&status);
    //kill(mChildPID, SIGCONT);
    wait(NULL);
#endif
    WD_END_LOG;

    return true;
}

void
MyProcess::setLimits(uint_64 aTimeLimit, uint_64 aMemoryLimit)
{
    mTimeLimit = aTimeLimit;
    mMemoryLimit = aMemoryLimit;
}

#ifdef LINUS_LINUX
//void
//MyProcess::handleContinueSignal(int sig)
//{
//    mIsPaused = false;
//}
//void handleContinueSignal2(int sig) {
//    myGlobalStaticContinueVariable = 1; // Or some other handling code
//}
#endif // LINUS_LINUX

std::pair<uint_64, uint_64>
MyProcess::runWithLimits()
//MyProcess::runWithLimits
//(
//    uint_64 aTimeLimit,
//    uint_64 aMemoryLimit
//)
{
    WD_LOG("Runing process with time and memory evaluation");
#if defined(BILL_WINDOWS)
    int reservedTime = 200;
    //long long startTime = getMillisecondsNow();
    long long startTime = getCPUTime();
#endif

    if (!run(mTimeLimit)) return { -1, -1 };

    //long long endTime = getMillisecondsNow();
#if defined(BILL_WINDOWS)
    long long endTime = getCPUTime();
    uint_64 timeUsage = endTime - startTime;
#else
    uint_64 timeUsage = 0;
#endif

#ifdef BILL_WINDOWS
    uint_64 memoryUsage = mFuture.get();
#else
    uint_64 memoryUsage = 0;
#endif // BILL_WINDOWS

    rusage resourseUsage;
    int status;
    wait4(mChildPID,&status,0,&resourseUsage);

    timeUsage += resourseUsage.ru_utime.tv_sec*1000000L;
    timeUsage += resourseUsage.ru_utime.tv_usec;
    timeUsage += resourseUsage.ru_stime.tv_sec*1000000L;
    timeUsage += resourseUsage.ru_stime.tv_usec;

    WD_LOG("time usage: " << timeUsage);
    WD_LOG("memory usage: " << memoryUsage);
    WD_END_LOG;

    return { timeUsage , memoryUsage };
}

void 
MyProcess::create(const std::vector<char*>& aParameters)
//(
//    std::string aName,
//    std::string aParameters
//)
{
    //aParameters = aName + aParameters;

    WD_LOG("Creating process name: " << aParameters[0]);
    //WD_LOG("Parameters: " << aParameters[1]);
#ifdef BILL_WINDOWS
    //std::string ss = aParameters[0];
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
        //res += std::string(aParameters[i]).c_str();
        //if (res.size() > 0) res += " ";
        strCopy(cmd, aParameters[i]);
        if (cmd[0] != 0) strCopy(cmd, " ");
    }
    //char* cmd = (char*) res.c_str();
    if (cmd[0] == 0) cmd = NULL;


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
        //WD_ERROR(process.0, (std::string("Can't start process ") + aParameters[0] +
        //    "\narguments are: " + aParameters[1]));
        WD_ERROR(process.0, (std::string("Can't start process ") + aParameters[0]));
    }
    delete name;
    delete cmd;
#endif
#ifdef LINUS_LINUX
    //signal(SIGCONT, MyProcess::handleContinueSignal);

    mChildPID = fork();
    if(mChildPID == -1)
    {
        WD_ERROR(process.linux.0, "Can't create process");
    }
    else if(!mChildPID)
    {
        //write(mChildPipes[1], aMessage.c_str(),  aMessage.size());
        dup2(mPipeA[0], STDIN_FILENO);
        dup2(mPipeB[1], STDOUT_FILENO);

        rlimit timeLimit;
        timeLimit.rlim_cur = mTimeLimit;
        timeLimit.rlim_max = mTimeLimit;
        setrlimit(RLIMIT_CPU,&timeLimit);

        rlimit memoryLimit;
        timeLimit.rlim_cur = mMemoryLimit;
        timeLimit.rlim_max = mMemoryLimit;
        setrlimit(RLIMIT_DATA,&timeLimit);

//        dup2(mChildPipes[0], STDIN_FILENO);
//        dup2(mParentPipes[1], STDOUT_FILENO);
//        signal(SIGCONT, handleContinueSignal2);
        //if (mIsPaused) pause();

        execvp(aParameters[0], &aParameters[0]);
    }
    else
    {

    }


/*
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
    }*/
    //char** param2 =
    //param.push_back(NULL);
   /* param[j] = NULL;
    execv((char*) aName.c_str(), param);
}*/
#endif
    WD_END_LOG;
}
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
#endif // BILL_WINDOWS

//--------------------------------------------------------------------------------

void
MyProcess::readPipe(std::string& result)
{
#ifdef PIPE_LOG_OUTPUT
    WD_LOG("Reading from pipe");
#endif // !PIPE_LOG_OUTPUT

#ifdef BILL_WINDOWS
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    unsigned long bread = 0;
    unsigned long avail = 0;

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
#elif defined(LINUS_LINUX)
    result.clear();
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    while(read(mPipeB[0], &buf, 1024) == 1024)
    {
        result += std::string(buf);
        memset(buf, 0, sizeof(buf));
    }
    result += std::string(buf);

//    char buf[1024];
//    while (read(mPipefd[0], &buf, 1) > 0)
//        write(STDOUT_FILENO, &buf, 1);
//    close(mPipefd[0]);
#endif

#ifdef PIPE_LOG_OUTPUT
    WD_END_LOG;
#endif // !PIPE_LOG_OUTPUT
}

void
MyProcess::writePipe(std::string& aMessage, PypeType aType)
{
#ifdef PIPE_LOG_OUTPUT
    WD_LOG("Writing from pipe");
#endif // !PIPE_LOG_OUTPUT

#ifdef BILL_WINDOWS
    unsigned long bread;
    //WriteFile(mThisSTDOUT, aMessage.c_str(), aMessage.size()
    //    + ((aType == ZERO) ? 1 : 0), &bread, NULL);
    WriteFile(mThisSTDOUT, aMessage.c_str(), aMessage.size(), &bread, NULL);
    if (aType == ZERO)
    {
        WriteFile(mThisSTDOUT, "\n", 1, &bread, NULL);
    }
#else
    //aMessage.push_back('\n');
    write(mPipeA[1], aMessage.c_str(),  aMessage.size());
    if (aType == ZERO)
    {
        write(mPipeA[1], "\n\0",  2);
    }
//    write(mPipefd[1], aMessage.c_str(), aMessage.size());
//    if (aType == ZERO)
//    {
//        write(mPipefd[1], "\n", 1);
//    }
//    close(mPipefd[1]);
#endif // BILL_WINDOWS

#ifdef PIPE_LOG_OUTPUT
    WD_LOG("write " + std::to_string(bread) + " bites\n");
    WD_END_LOG;
#endif // !PIPE_LOG_OUTPUT

}

void
MyProcess::IORedirection()
{
    WD_LOG("Rederecting input and output to pipe");

#ifdef BILL_WINDOWS
    SECURITY_ATTRIBUTES securatyAttributes;
    securatyAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
    securatyAttributes.lpSecurityDescriptor = NULL;
    securatyAttributes.bInheritHandle = true;

    if (!CreatePipe(&mChildSTDIN, &mThisSTDOUT, &securatyAttributes, 0))
    {
        WD_ERROR(process.pipe.0, "Can't create pipe ");
    }

    if (!CreatePipe(&mThisSTDIN, &mChildSTDOUT, &securatyAttributes, 0))
    {
        WD_ERROR(process.pipe.1, "Can't create pipe ");
    }
    //else if (aType == Process::IOType::MIXED) {}

    //GetStartupInfo(&mStartupInfo);
    ZeroMemory(&mStartupInfo, sizeof(STARTUPINFO));
    mStartupInfo.cb = sizeof(STARTUPINFO);
    mStartupInfo.dwFlags |= STARTF_USESTDHANDLES;

    mStartupInfo.hStdInput = mChildSTDIN;
    mStartupInfo.hStdError = mChildSTDOUT;
    mStartupInfo.hStdOutput = mChildSTDOUT;
#elif defined(LINUS_LINUX)
    pipe(mPipeA);
    pipe(mPipeB);
#endif // BILL_WINDOWS

    WD_END_LOG;
}

void
MyProcess::closeHandles()
{
#ifdef BILL_WINDOWS
    CloseHandle(mChildSTDIN);
    CloseHandle(mChildSTDOUT);
#endif // BILL_WINDOWS
}

//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------