//--------------------------------------------------------------------------------
#include "process/pipe_process.hpp"

PipeProcess::PipeProcess
(
    const std::vector<char*>& aParameters,
    uint_64 aTimeLimit,
    uint_64 aMemoryLimit
) :
    MyProcess(aParameters)
{
    if (aTimeLimit != 1e18 && aMemoryLimit != 1e18)
    {
        setLimits(aTimeLimit, aMemoryLimit);
    }

    IORedirection();
    create(aParameters);
}

PipeProcess::~PipeProcess() 
{
    closeHandles();
}

#ifdef LINUS_LINUX
void
MyProcess::create(const std::vector<char*>& aParameters)
{
    //aParameters = aName + aParameters;

    WD_LOG("Creating process name: " << aParameters[0]);
    //WD_LOG("Parameters: " << aParameters[1]);
    //signal(SIGCONT, MyProcess::handleContinueSignal);

    mChildPID = fork();
    if (mChildPID == -1)
    {
        WD_ERROR(process.linux.0, "Can't create process");
    }
    else if (!mChildPID)
    {
        //write(mChildPipes[1], aMessage.c_str(),  aMessage.size());

//        fcntl(mPipeA[0], F_SETPIPE_SZ, 65336 * 2);
//        fcntl(mPipeA[1], F_SETPIPE_SZ, 65336 * 2);
//        fcntl(mPipeB[0], F_SETPIPE_SZ, 65336 * 2);
//        fcntl(mPipeB[1], F_SETPIPE_SZ, 65336 * 2);

        dup2(mPipeA[0], STDIN_FILENO);
        dup2(mPipeB[1], STDOUT_FILENO);

        //        fcntl(mPipeA[0], F_SETPIPE_SZ, 65336 * 2);
        //        fcntl(mPipeA[1], F_SETPIPE_SZ, 65336 * 2);
        //        fcntl(mPipeB[0], F_SETPIPE_SZ, 65336 * 2);
        //        fcntl(mPipeB[1], F_SETPIPE_SZ, 65336 * 2);

        rlimit timeLimit;
        timeLimit.rlim_cur = mTimeLimit;
        timeLimit.rlim_max = mTimeLimit;
        setrlimit(RLIMIT_CPU, &timeLimit);

        rlimit memoryLimit;
        memoryLimit.rlim_cur = mMemoryLimit;
        memoryLimit.rlim_max = mMemoryLimit;
        setrlimit(RLIMIT_STACK, &memoryLimit);

        //        dup2(mChildPipes[0], STDIN_FILENO);
        //        dup2(mParentPipes[1], STDOUT_FILENO);
        //        signal(SIGCONT, handleContinueSignal2);
                //if (mIsPaused) pause();

        execvp(aParameters[0], &aParameters[0]);
        //WD_ERROR(linux.exec.0, "cant start process");
    }
    else
    {
        //        fcntl(mPipeA[0], F_SETPIPE_SZ, 65336 * 2);
        //        fcntl(mPipeA[1], F_SETPIPE_SZ, 65336 * 2);
        //        fcntl(mPipeB[0], F_SETPIPE_SZ, 65336 * 2);
        //        fcntl(mPipeB[1], F_SETPIPE_SZ, 65336 * 2);
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
    WD_END_LOG;
}
#endif

#define BUFFER_SIZE 65336 * 10

void
PipeProcess::IORedirection()
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


    fcntl(mPipeA[0], F_SETPIPE_SZ, BUFFER_SIZE);
    fcntl(mPipeA[1], F_SETPIPE_SZ, BUFFER_SIZE);
    fcntl(mPipeB[0], F_SETPIPE_SZ, BUFFER_SIZE);
    fcntl(mPipeB[1], F_SETPIPE_SZ, BUFFER_SIZE);

#endif // BILL_WINDOWS

    WD_END_LOG;
}


void
PipeProcess::readPipe(std_string_ref result)
{
#ifdef PIPE_LOGS
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
    while (read(mPipeB[0], &buf, 1024) == 1024)
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

#ifdef PIPE_LOGS
    WD_END_LOG;
#endif // !PIPE_LOG_OUTPUT
}

void
PipeProcess::writePipe(std_string_ref aMessage, PypeType aType)
{
#ifdef PIPE_LOGS
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
    write(mPipeA[1], aMessage.c_str(), aMessage.size());
    if (aType == ZERO)
    {
        write(mPipeA[1], "\n\0", 2);
    }
    //    write(mPipefd[1], aMessage.c_str(), aMessage.size());
    //    if (aType == ZERO)
    //    {
    //        write(mPipefd[1], "\n", 1);
    //    }
    //    close(mPipefd[1]);
#endif // BILL_WINDOWS

#ifdef PIPE_LOGS
    WD_LOG("write " + std::to_string(bread) + " bites\n");
    WD_END_LOG;
#endif // !PIPE_LOG_OUTPUT

}


void
PipeProcess::closeHandles()
{
#ifdef BILL_WINDOWS
    CloseHandle(mChildSTDIN);
    CloseHandle(mChildSTDOUT);
#endif // BILL_WINDOWS
}

//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------