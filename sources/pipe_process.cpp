//--------------------------------------------------------------------------------

#include "pipe_process.h"

PipeProcess::PipeProcess(std::string& aName, std::string& aParameters) :
    MyProcess     (aName, aParameters)
{
    IORedirection();
    create(aName, aParameters);
}

PipeProcess::~PipeProcess() {}

void
PipeProcess::readPipe(std::string& result)
{
#ifdef PIPE_LOG_OUTPUT
    WD_LOG("Reading from pipe");
#endif PIPE_LOG_OUTPUT

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
#else
    char buf[1024];
    while (read(mPipefd[0], &buf, 1) > 0)
        write(STDOUT_FILENO, &buf, 1);
    close(mPipefd[0]);
#endif

#ifdef PIPE_LOG_OUTPUT
    WD_END_LOG;
#endif PIPE_LOG_OUTPUT
}

void
PipeProcess::writePipe(std::string& aMessage, PypeType aType)
{
#ifdef PIPE_LOG_OUTPUT
    WD_LOG("Writing from pipe");
#endif

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
    write(mPipefd[1], aMessage.c_str(), aMessage.size());
    if (aType == ZERO)
    {
        write(mPipefd[1], "\n", 1);
    }
    close(mPipefd[1]);
#endif // BILL_WINDOWS

#ifdef PIPE_LOG_OUTPUT
    WD_LOG("write " + std::to_string(bread) + " bites\n");
    WD_END_LOG;
#endif PIPE_LOG_OUTPUT

}

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
#else

#endif // BILL_WINDOWS

    WD_END_LOG;
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