//--------------------------------------------------------------------------------

#include "file_process.h"

FileProcess::FileProcess
(
    std::string& aName,
    std::string& aParameters,
    std::string& aInputPath,
    std::string& aOutputPath
) :
    MyProcess     (aName, aParameters),
    mInputPath  (aInputPath),
    mOutputPath (aOutputPath)
{
    IORedirection();
    create(aName, aParameters);
}

FileProcess::~FileProcess() {}
void MyProcess::IORedirection() {}
void
FileProcess::IORedirection()
{
    SECURITY_ATTRIBUTES securatyAttributes;
    securatyAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
    securatyAttributes.lpSecurityDescriptor = NULL;
    securatyAttributes.bInheritHandle = true;

    WD_LOG("Rederecting input to: " << mInputPath);
    WD_LOG("Rederecting output to: " << mOutputPath);

    if (mInputPath != "")
    {
        mInputFileHandle = CreateFileA(mInputPath.c_str(),
            GENERIC_READ,
            FILE_SHARE_READ,
            &securatyAttributes,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
    }

    if (mOutputPath != "")
    {
        mOutputFileHandle = CreateFileA(mOutputPath.c_str(),
            FILE_WRITE_DATA,
            FILE_SHARE_WRITE,
            &securatyAttributes,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
    }

    //GetStartupInfo(&mStartupInfo);
    ZeroMemory(&mStartupInfo, sizeof(STARTUPINFO));
    mStartupInfo.cb = sizeof(STARTUPINFO);
    mStartupInfo.dwFlags |= STARTF_USESTDHANDLES;

    mStartupInfo.hStdInput = mInputFileHandle;
    mStartupInfo.hStdError = mOutputFileHandle;
    mStartupInfo.hStdOutput = mOutputFileHandle;

    WD_END_LOG;
}

void
FileProcess::closeHandles()
{
    CloseHandle(mInputFileHandle);
    CloseHandle(mOutputFileHandle);
}

//--------------------------------------------------------------------------------