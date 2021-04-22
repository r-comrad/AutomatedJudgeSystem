#include <windows.h>
#include <string>
#include <iostream>

//void IORedirection(STARTUPINFO& aStartInfo, std::wstring& aInputPath, std::wstring& aOutputPath)
//{
//    SECURITY_ATTRIBUTES sa;
//    sa.nLength = sizeof(sa);
//    sa.lpSecurityDescriptor = NULL;
//    sa.bInheritHandle = TRUE;
//
//    HANDLE inputHandle = CreateFile(aInputPath.c_str(),
//        GENERIC_READ,
//        FILE_SHARE_READ,
//        &sa,
//        OPEN_EXISTING,
//        FILE_ATTRIBUTE_NORMAL,
//        NULL);
//
//    HANDLE outputHandle = CreateFile(aOutputPath.c_str(),
//        FILE_APPEND_DATA,
//        FILE_SHARE_WRITE,
//        &sa,
//        OPEN_ALWAYS,
//        FILE_ATTRIBUTE_NORMAL,
//        NULL);
//
//    PROCESS_INFORMATION pi;
//    BOOL ret = FALSE;
//    DWORD flags = 0;// CREATE_NO_WINDOW;
//
//    ZeroMemory(&aStartInfo, sizeof(STARTUPINFO));
//    aStartInfo.cb = sizeof(STARTUPINFO);
//    aStartInfo.dwFlags |= STARTF_USESTDHANDLES;
//    aStartInfo.hStdInput = inputHandle;
//    aStartInfo.hStdError = outputHandle;
//    aStartInfo.hStdOutput = outputHandle;
//}

//int main(int argc, char* argv[])
//{
//    std::wstring programPath    = L"D:\\projects\\VS 2019\\ChiniseTester\\Resources\\YesInput.exe";
//    std::wstring inputPath      = L"D:\\projects\\VS 2019\\ChiniseTester\\Resources\\input.txt";
//    std::wstring outputPath     = L"D:\\projects\\VS 2019\\ChiniseTester\\Resources\\output.txt";
//
//
//    PROCESS_INFORMATION pi;
//    STARTUPINFO si;
//    BOOL ret = FALSE;
//    DWORD flags = 0;// CREATE_NO_WINDOW;
//
//    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
//    IORedirection(si, inputPath, outputPath);
//
//    TCHAR cmd[] = TEXT("D:\\projects\\VS 2019\\ChiniseTester\\Resources\\YesInput3.exe");
//    ret = CreateProcess(NULL, cmd, NULL, NULL, TRUE, flags, NULL, NULL, &si, &pi);
//
//    if (ret)
//    {
//        CloseHandle(pi.hProcess);
//        CloseHandle(pi.hThread);
//        return 0;
//    }
//
//    return -1;
//}