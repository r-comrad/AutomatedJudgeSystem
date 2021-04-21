#pragma GCC diagnostic ignored "-Wwrite-strings"
#define INT_MAX 2147483647

#include <cstdint> 
#include <future>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <thread>

#include <windows.h>
#include <userenv.h>
#include <psapi.h>
#include <tlhelp32.h>

/**
 * Function Prototypes.
 */
bool setupIoRedirection(std::wstring, std::wstring, HANDLE&, HANDLE&);
void setupStartupInfo(STARTUPINFOW&, HANDLE&, HANDLE&);
bool createProcess(const std::wstring&, const std::wstring&, const std::wstring&, HANDLE&, LPVOID, STARTUPINFOW&, PROCESS_INFORMATION&);
DWORD runProcess(PROCESS_INFORMATION&, long long, long long, long long&, long long&);
long long getMaxMemoryUsage(PROCESS_INFORMATION&, long long);
long long getCurrentMemoryUsage(HANDLE&);
long long getMillisecondsNow();
bool killProcess(PROCESS_INFORMATION&);
DWORD getExitCode(HANDLE&);
std::string getErrorMessage(const std::string&);
std::wstring getWideString(const std::string&);
LPWSTR getWideStringPointer(const std::wstring&);
LPCWSTR getConstWideStringPointer(const std::wstring&);

/**
 * JNI调用入口.
 * 获取程序运行结果.
 * @param  jniEnv          - JNI 运行环境引用
 * @param  selfReference   - 对调用Java的对象的引用
 * @param  jCommandLine    - 待执行的命令行
 * @param  jUsername       - Windows用户名
 * @param  jPassword       - Windows密码
 * @param  jInputFilePath  - 执行程序时的输入文件路径(可为NULL)
 * @param  jOutputFilePath - 执行程序后的输出文件路径(可为NULL)
 * @param  timeLimit       - 程序执行时间限制(ms, 0为不限制)
 * @param  memoryLimit     - 程序执行内存限制(KB, 0为不限制)
 * @return 一个包含运行结果的Map<String, Object>对象
 */
void foo(
    std::string jCommandLine, std::string jUsername,
    std::string jPassword, std::string jInputFilePath, std::string jOutputFilePath, long long timeLimit,
    long long memoryLimit) {
    std::wstring        commandLine = getWideString(jCommandLine);
    std::wstring        username = getWideString(jUsername);
    std::wstring        password = getWideString(jPassword);
    std::wstring        inputFilePath = getWideString(jInputFilePath);
    std::wstring        outputFilePath = getWideString(jOutputFilePath);

    HANDLE              hInput = { 0 };
    HANDLE              hOutput = { 0 };
    HANDLE              hToken = { 0 };
    LPVOID              lpEnvironment = NULL;
    PROCESS_INFORMATION processInfo = { 0 };
    STARTUPINFOW        startupInfo = { 0 };

    //JHashMap            result;
    long long                timeUsage = 0;
    long long                memoryUsage = 0;
    DWORD               exitCode = 127;

    if (!setupIoRedirection(inputFilePath, outputFilePath, hInput, hOutput)) {
        //throwStringException(jniEnv, getErrorMessage("SetupIoRedirection"));
        return;
    }
    setupStartupInfo(startupInfo, hInput, hOutput);

    if (!createProcess(commandLine, username, password, hToken, lpEnvironment, startupInfo, processInfo)) {
        //throwStringException(jniEnv, getErrorMessage("CreateProcess"));
        return;
    }

    exitCode = runProcess(processInfo, timeLimit, memoryLimit, timeUsage, memoryUsage);
    CloseHandle(hInput);
    CloseHandle(hOutput);

    //result.put("usedTime", timeUsage);
    //result.put("usedMemory", memoryUsage);
    //result.put("exitCode", exitCode);
    std::cout << timeUsage << std::endl;
    std::cout << memoryUsage << std::endl;
    std::cout << exitCode << std::endl;

    //return result.toJObject(jniEnv);
}

/**
 * 重定向子进程的I/O.
 * @param inputFilePath  - 输入文件路径
 * @param outputFilePath - 输出文件路径
 * @param hInput         - 输入文件句柄
 * @param hOutput        - 输出文件句柄
 */
bool setupIoRedirection(std::wstring inputFilePath, std::wstring outputFilePath,
    HANDLE& hInput, HANDLE& hOutput) {
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    if (!inputFilePath.empty()) {
        hInput = CreateFileW(inputFilePath.c_str(), GENERIC_READ, 0, &sa,
            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hInput == INVALID_HANDLE_VALUE) {
            return false;
        }
    }
    if (!outputFilePath.empty()) {
        hOutput = CreateFileW(outputFilePath.c_str(), GENERIC_WRITE, 0, &sa,
            CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hInput == INVALID_HANDLE_VALUE) {
            return false;
        }
    }
    return true;
}

/**
 * 根据I/O重定向信息重新设置startupInfo.
 * @param startupInfo - STARTUPINFOW结构体
 * @param hInput      - 文件输入句柄
 * @param hOutput     - 文件输出句柄
 */
void setupStartupInfo(STARTUPINFOW& startupInfo, HANDLE& hInput, HANDLE& hOutput) {
    startupInfo.cb = sizeof(STARTUPINFOW);
    startupInfo.dwFlags |= STARTF_USESTDHANDLES;
    startupInfo.hStdInput = hInput;
    startupInfo.hStdError = hOutput;
    startupInfo.hStdOutput = hOutput;
}

/**
 * 创建进程.
 * @param  commandLine   - 待执行的命令行
 * @param  username      - Windows用户名
 * @param  password      - Windows密码
 * @param  hToken        - a token that represents the specified user
 * @param  lpEnvironment - an environment block for the new process
 * @param  startupInfo   - a STARTUPINFO structure
 * @param  processInfo   - a PROCESS_INFORMATION structure that receives identification
 *                         information for the new process, including a handle to the process
 * @return 进程是否创建成功
 */
 //TODO: ERRORS
bool createProcess(const std::wstring& commandLine, const std::wstring& username,
    const std::wstring& password, HANDLE& hToken, LPVOID lpEnvironment,
    STARTUPINFOW& startupInfo, PROCESS_INFORMATION& processInfo) {
    WCHAR   szUserProfile[256] = L"";
    DWORD   dwSize = sizeof(szUserProfile) / sizeof(WCHAR);
    LPCWSTR lpUsername = getConstWideStringPointer(username);
    LPCWSTR lpDomain = getConstWideStringPointer(L".");
    LPCWSTR lpPassword = getConstWideStringPointer(password);
    LPWSTR  lpCommandLine = getWideStringPointer(commandLine);

    //if (!LogonUserW(lpUsername, lpDomain, lpPassword, LOGON32_LOGON_BATCH,
    //    LOGON32_PROVIDER_DEFAULT, &hToken)) {
    //    return false;
    //}


    //if (!CreateEnvironmentBlock(&lpEnvironment, hToken, TRUE)) {
    //    return false;
    //}
    //if (!GetUserProfileDirectoryW(hToken, szUserProfile, &dwSize)) {
    //    return false;
    //}
    //if (!CreateProcessWithLogonW(lpUsername, lpDomain, lpPassword,
    //    LOGON_WITH_PROFILE, NULL, lpCommandLine,
    //    CREATE_UNICODE_ENVIRONMENT | CREATE_SUSPENDED | CREATE_NO_WINDOW,
    //    lpEnvironment, szUserProfile, &startupInfo, &processInfo)) {
    //    return false;
    //}


    if (!CreateProcessWithLogonW(lpUsername, lpDomain, lpPassword,
        LOGON_WITH_PROFILE, NULL, lpCommandLine,
        CREATE_UNICODE_ENVIRONMENT | CREATE_SUSPENDED | CREATE_NO_WINDOW,
        NULL, szUserProfile, &startupInfo, &processInfo)) {
        return false;
    }
    return true;
}

/**
 * 运行进程.
 * @param  processInfo - 包含进程信息的PROCESS_INFORMATION结构体
 * @param  timeLimit   - 运行时时间限制(ms)
 * @param  memoryLimit - 运行时空间限制(KB)
 * @param  timeUsage   - 运行时时间占用(ms)
 * @param  memoryUsage - 运行时空间占用(ms)
 * @return 进程退出状态
 */
DWORD runProcess(PROCESS_INFORMATION& processInfo, long long timeLimit,
    long long memoryLimit, long long& timeUsage, long long& memoryUsage) {
    int  reservedTime = 200;
    auto feature = std::async(std::launch::async, getMaxMemoryUsage, std::ref(processInfo), memoryLimit);

    ResumeThread(processInfo.hThread);
    long long startTime = getMillisecondsNow();
    WaitForSingleObject(processInfo.hProcess, timeLimit + reservedTime);
    long long endTime = getMillisecondsNow();
    timeUsage = endTime - startTime;

    if (getExitCode(processInfo.hProcess) == STILL_ACTIVE) {
        killProcess(processInfo);
    }
    memoryUsage = feature.get();

    return getExitCode(processInfo.hProcess);
    return 0;
}

/**
 * 获取运行时内存占用最大值
 * @param  processInfo - 包含进程信息的PROCESS_INFORMATION结构体
 * @param  memoryLimit - 运行时空间限制(KB)
 * @return 运行时内存占用最大值
 */
long long getMaxMemoryUsage(PROCESS_INFORMATION& processInfo, long long memoryLimit) {
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

/**
 * 获取内存占用情况.
 * @param  hProcess - 进程句柄
 * @return 当前物理内存使用量(KB)
 */
long long getCurrentMemoryUsage(HANDLE& hProcess) {
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

/**
 * 获取当前系统时间.
 * 用于统计程序运行时间.
 * @return 当前系统时间(以毫秒为单位)
 */
long long getMillisecondsNow() {
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

/**
 * 强制销毁进程(当触发阈值时).
 * @param  processInfo - 包含进程信息的PROCESS_INFORMATION结构体
 * @return 销毁进程操作是否成功完成
 */
bool killProcess(PROCESS_INFORMATION& processInfo) {
    DWORD           processId = processInfo.dwProcessId;
    PROCESSENTRY32 processEntry = { 0 };
    processEntry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE handleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (Process32First(handleSnap, &processEntry)) {
        BOOL isContinue = TRUE;

        do {
            if (processEntry.th32ParentProcessID == processId) {
                HANDLE hChildProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processEntry.th32ProcessID);
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
    return true;
}

/**
 * 获取应用程序退出状态.
 * 0表示正常退出, 259表示仍在运行.
 * @param  hProcess - 进程的句柄
 * @return 退出状态
 */
DWORD getExitCode(HANDLE& hProcess) {
    DWORD exitCode = 0;
    GetExitCodeProcess(hProcess, &exitCode);

    return exitCode;
}

/**
 * 获取Windows API异常信息.
 * @param  apiName - Windows API名称
 * @return Windows API异常信息
 */
std::string getErrorMessage(const std::string& apiName) {
    LPVOID lpvMessageBuffer;

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&lpvMessageBuffer, 0, NULL);

    std::stringstream stringStream;
    std::string errorMessage((LPSTR)lpvMessageBuffer);

    stringStream << "API:     " << apiName << std::endl
        << "Code:    " << GetLastError() << std::endl
        << "Message: " << errorMessage << std::endl;
    LocalFree(lpvMessageBuffer);

    return stringStream.str();
    return "";
}

/**
 * 获取std::wstring类型的字符串.
 * @param  str - std::string类型的字符串
 * @return wstring类型的字符串
 */
std::wstring getWideString(const std::string& str) {
    std::wstring wstr(str.begin(), str.end());
    return wstr;
}

/**
 * 获取std::wstring对应LPWSTR类型的指针.
 * @param  str - std::wstring类型的字符串
 * @return 对应LPWSTR类型的指针
 */
LPWSTR getWideStringPointer(const std::wstring& str) {
    return const_cast<LPWSTR>(str.c_str());
}

/**
 * 获取std::wstring对应LPCWSTR类型的指针.
 * @param  str - std::wstring类型的字符串
 * @return 对应LPCWSTR类型的指针
 */
LPCWSTR getConstWideStringPointer(const std::wstring& str) {
    return str.c_str();
}

void myJudger(std::wstring aName, std::wstring aInputFilePath, std::wstring aOutputFilePath)
{
    LPCWSTR name = aName.c_str();
    LPCWSTR inputFilePath = aInputFilePath.c_str();
    LPCWSTR outputFilePath = aOutputFilePath.c_str();

    long long timeUsage = 1000;
    long long reservedTime = 200;

    HANDLE              hInput = { 0 };
    HANDLE              hOutput = { 0 };
    STARTUPINFOW        startupInfo = { 0 };



    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    //STARTUPINFO cif;

    ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    startupInfo.dwFlags |= STARTF_USESTDHANDLES;

    if (!setupIoRedirection(inputFilePath, outputFilePath, hInput, hOutput))
    {
        std::cout << "ERROR #1: can't redirection paths" << std::endl;
    }

    setupStartupInfo(startupInfo, hInput, hOutput);

    // long long startTime = getMillisecondsNow();
  /*   if (!CreateProcess(name, NULL,
         NULL, NULL, FALSE, CREATE_UNICODE_ENVIRONMENT | CREATE_SUSPENDED | CREATE_NO_WINDOW, NULL, NULL, &startupInfo, &pi) == TRUE)*/
    if (!CreateProcess(name, NULL,
        NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &startupInfo, &pi) == TRUE)
    {
        std::cout << "ERROR #2: can't start process" << std::endl;
    }
    ResumeThread(pi.hThread);
    WaitForSingleObject(pi.hProcess, 1000);
    //WaitForSingleObject(pi.hProcess, timeUsage + reservedTime);
    //long long endTime = getMillisecondsNow();

    CloseHandle(hInput);
    CloseHandle(hOutput);

    //std::cout << "time usage: " << endTime - startTime;

}

#include <vector>
#include <iostream>
#include <string>
int mmain()
{
    std::wstring programPath = L"D:\\projects\\VS 2019\\ChiniseTester\\Resources\\YesInput.exe";
    std::wstring inputPath = L"D:\\projects\\VS 2019\\ChiniseTester\\Resources\\input.txt";
    std::wstring outputPath = L"D:\\projects\\VS 2019\\ChiniseTester\\Resources\\input.txt";
    myJudger(programPath, inputPath, outputPath);

    //foo("U:\\_Private\\Tester\\Programs\\b.exe", NULL,
    //    NULL, NULL, NULL, 1000,
    //    1000);
    //foo("c:\\c.exe", "",
    //    "", "", "", 1000,
    //    1000);

    //STARTUPINFO cif;
    //ZeroMemory(&cif, sizeof(STARTUPINFO));
    //PROCESS_INFORMATION pi;
    //if (CreateProcess(L"c:\\c.exe", NULL,
    //    NULL, NULL, FALSE, NULL, NULL, NULL, &cif, &pi) == TRUE)
    //{
    //    //cout << "process" << endl;
    //    //cout << "handle " << pi.hProcess << endl;
    //    //Sleep(1000);                // подождать
    //    //TerminateProcess(pi.hProcess, NO_ERROR);    // убрать процесс
    //}

    //long long                timeUsage = 1000;
    //long long                memoryUsage = 0;
    //int  reservedTime = 0;
    //WaitForSingleObject(pi.hProcess, timeUsage + reservedTime);
    //TerminateProcess(pi.hProcess, NO_ERROR);

    //STARTUPINFO cif;
    //ZeroMemory(&cif, sizeof(STARTUPINFO));
    //PROCESS_INFORMATION pi;
    //if (CreateProcess(L"c:\\windows\\notepad.exe", NULL,
    //    NULL, NULL, FALSE, NULL, NULL, NULL, &cif, &pi) == TRUE)
    //{
    //    std::cout << "process" << std::endl;
    //    std::cout << "handle " << pi.hProcess << std::endl;
    //    Sleep(10000);                // подождать
    //    TerminateProcess(pi.hProcess, NO_ERROR);    // убрать процесс
    //}

    return 0;
}