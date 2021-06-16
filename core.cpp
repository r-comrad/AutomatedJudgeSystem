#include "core.h"

Core::Core()    :
    mStartupInfo({ 0 })
{
    ZeroMemory(&processInfo, sizeof(PROCESS_INFORMATION));
}

void Core::IORedirection(std::wstring& aInputPath, std::wstring& aOutputPath)
{
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    mInputHandle = CreateFile(aInputPath.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        &sa,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    mOutputHandle = CreateFile(aOutputPath.c_str(),
        FILE_APPEND_DATA,
        FILE_SHARE_WRITE,
        &sa,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    PROCESS_INFORMATION pi;
    BOOL ret = FALSE;
    DWORD flags = 0;// CREATE_NO_WINDOW;

    ZeroMemory(&mStartupInfo, sizeof(STARTUPINFO));
    mStartupInfo.cb = sizeof(STARTUPINFO);
    mStartupInfo.dwFlags |= STARTF_USESTDHANDLES;
    mStartupInfo.hStdInput = mInputHandle;
    mStartupInfo.hStdError = mOutputHandle;
    mStartupInfo.hStdOutput = mOutputHandle;
}

void Core::runProcess(std::wstring aName, std::wstring aInputFilePath, std::wstring aOutputFilePath) {
    IORedirection(aInputFilePath, aOutputFilePath);
    wchar_t* cmd = const_cast<wchar_t*>(aName.c_str());

    auto feature = std::async(std::launch::async, getMaxMemoryUsage, std::ref(mProcessInfo), 1000000);

    if (CreateProcess(NULL, cmd,
        //   NULL, NULL, TRUE, CREATE_UNICODE_ENVIRONMENT | CREATE_SUSPENDED | CREATE_NO_WINDOW, NULL, NULL, &startupInfo, &processInfo) == FALSE)
        NULL, NULL, TRUE, CREATE_UNICODE_ENVIRONMENT | CREATE_SUSPENDED | CREATE_NO_WINDOW, NULL, NULL, &startupInfo, &processInfo) == FALSE)
    {
        #ifdef DBG
        std::cout << "ERROR #2: can't start process" << std::endl;
        #endif // DEBUG      
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

    CloseHandle(mInputHandle);
    CloseHandle(mOutputHandle);
}