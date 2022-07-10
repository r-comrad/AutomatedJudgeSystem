//--------------------------------------------------------------------------------
#include "pipe_windows_process.hpp"

proc::PipeWindowsProcess::PipeWindowsProcess() noexcept
{
    IORedirection();
}

proc::PipeWindowsProcess::~PipeWindowsProcess() 
{
    closeHandles();
}

#define BUFFER_SIZE 65336 * 10

void
proc::PipeWindowsProcess::IORedirection() noexcept
{
    WRITE_LOG("Rederecting_input_and_output_to_pipe");

    SECURITY_ATTRIBUTES securatyAttributes;
    securatyAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
    securatyAttributes.lpSecurityDescriptor = NULL;
    securatyAttributes.bInheritHandle = true;

    if (!CreatePipe(&mChildSTDIN, &mThisSTDOUT, &securatyAttributes, 0))
    {
        WRITE_ERROR("PipeProcess", "IORedirection", 20, "Can't_create_pipe", "Windows");
    }

    if (!CreatePipe(&mThisSTDIN, &mChildSTDOUT, &securatyAttributes, 0))
    {
        WRITE_ERROR("PipeProcess", "IORedirection", 21, "Can't_create_pipe", "Windows");
    }
    //else if (aType == Process::IOType::MIXED) {}

    //GetStartupInfo(&mStartupInfo);
    ZeroMemory(&mStartupInfo, sizeof(STARTUPINFO));
    mStartupInfo.cb = sizeof(STARTUPINFO);
    mStartupInfo.dwFlags |= STARTF_USESTDHANDLES;

    mStartupInfo.hStdInput = mChildSTDIN;
    mStartupInfo.hStdError = mChildSTDOUT;
    mStartupInfo.hStdOutput = mChildSTDOUT;
}


void
proc::PipeWindowsProcess::readPipe(std::string& result) noexcept
{
#ifdef PIPE_LOGS
    WRITE_LOG("Reading_from_pipe");
#endif // !PIPE_LOG_OUTPUT

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

#ifdef PIPE_LOGS
    WD_END_LOG;
#endif // !PIPE_LOG_OUTPUT
}

void
proc::PipeWindowsProcess::writePipe(std::string& aMessage, PypeType aType) noexcept
{
#ifdef PIPE_LOGS
    WRITE_LOG("Writing_from_pipe");
#endif // !PIPE_LOG_OUTPUT

    unsigned long bread;
    //WriteFile(mThisSTDOUT, aMessage.c_str(), aMessage.size()
    //    + ((aType == ZERO) ? 1 : 0), &bread, NULL);
    WriteFile(mThisSTDOUT, aMessage.c_str(), aMessage.size(), &bread, NULL);
    if (aType == ZERO)
    {
        WriteFile(mThisSTDOUT, "\n", 1, &bread, NULL);
    }

#ifdef PIPE_LOGS
    WD_LOG("write " + std::to_string(bread) + " bites\n");
    WD_END_LOG;
#endif // !PIPE_LOG_OUTPUT

}


void
proc::PipeWindowsProcess::closeHandles() noexcept
{
    CloseHandle(mChildSTDIN);
    CloseHandle(mChildSTDOUT);
}

//--------------------------------------------------------------------------------