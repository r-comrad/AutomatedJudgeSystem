#include <windows.h>
#include <string>
#include <iostream>

int main(int argc, char* argv[])
{
    std::wstring programPath = L"D:\\projects\\VS 2019\\ChiniseTester\\Resources\\YesInput.exe";
    std::wstring inputPath = L"D:\\projects\\VS 2019\\ChiniseTester\\Resources\\input.txt";
    std::wstring outputPath = L"D:\\projects\\VS 2019\\ChiniseTester\\Resources\\output.txt";

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    HANDLE h = CreateFile(outputPath.c_str(),
        FILE_APPEND_DATA,
        FILE_SHARE_WRITE,
        &sa,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    SECURITY_ATTRIBUTES sas;
    sas.nLength = sizeof(sas);
    sas.lpSecurityDescriptor = NULL;
    sas.bInheritHandle = TRUE;

    HANDLE hh = CreateFile(inputPath.c_str(),
        FILE_APPEND_DATA,
        FILE_SHARE_READ,
        &sas,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    BOOL ret = FALSE;
    DWORD flags = 0;// CREATE_NO_WINDOW;

    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.dwFlags |= STARTF_USESTDHANDLES;
    si.hStdInput = hh;
    si.hStdError = h;
    si.hStdOutput = h;

    TCHAR cmd[] = TEXT("D:\\projects\\VS 2019\\ChiniseTester\\Resources\\YesInput.exe");
    ret = CreateProcess(NULL, cmd, NULL, NULL, TRUE, flags, NULL, NULL, &si, &pi);

    if (ret)
    {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return 0;
    }

    return -1;
}