#include "database.h"
#include "run.h"
#include "database_query.h"

#include <map>

void makeChecker(std::wstring aTaskName)
{
    std::wstring TEST = getMainPath() + RESOURCES + L"MikeMirzayanov.cpp";
    std::wstring checkerkath = getMainPath() + RESOURCES + aTaskName + L"\\checker\\";

    std::ifstream fromFile(TEST);
    std::wstring sss = checkerkath + L"checker.cpp";
    std::wstring checkerEXE = checkerkath + L"checker.exe";
    std::ofstream toFile(sss);
    
    std::string s;
    //while (fromFile >> c) toFile << c;
    //while (fscanf(fromFile, "%c", &c)) toFile << c;
    while (std::getline(fromFile, s)) toFile << s << '\n';
    Process process;
    process.IORedirection(L"", L"");
    //C:\Windows\SysWOW64\WindowsPowerShell\v1.0\\powershell.exe

    //std::wstring answerAddress = curPath + L"answer\\" + std::to_wstring(testNum);
    //std::wstring resultAddress = curPath + L"result\\" + std::to_wstring(testNum);
    //std::wstring parameters = L"sas input " + outAddress + L" " + answerAddress;


        //C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\Launch-VsDevShell.ps1
      //  run.runProcess( L"C:\\Windows\\SysWOW64\\WindowsPowerShell\\v1.0\\powershell.exe",
        //run.runProcess( L"C:\\VsDevCmd2.bat",
      //      L"", L"", L"cd " + checkerkath + L"\n" + L"cl /EHsc checker.cpp");
       //     L"", L"", L"C:\\Windows\\SysWOW64\\WindowsPowerShell\\v1.0\\powershell.exe cl /EHsc checker.cpp");


    //run.runProcess(L"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\Tools\\VsDevCmd.bat & cl /EHsc " + sss, L"", L"", L"");
    
    
    // cd /d U:
    // cd -Path U:
    //std::string ssss = "\"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\Tools\\VsDevCmd.bat\" & cl /EHsc \""

    std::string compilerPath = "\"C:\\Program Files(x86)\\Microsoft\ Visual\ Studio\\2019\Community\\Common7\Tools\\VsDevCmd.bat\"";
    std::string compilerComand = " cl /EHsc ";
    //std::string compilerComand = "\"cl /EHsc\" ";
    std::string checkerPath = "\"D:\\projects\\VS_2019\\ChineseTester\\resources\\task2\\checker\\checker.cpp\"";
    std::string outPath = "/FoD:\\projects\\VS_2019\\ChineseTester\\resources\\task2\\checker\\checker.exe";


    std::string ssss = compilerPath + compilerComand + checkerPath + " " + outPath;
    std::string ssss1 = compilerComand + checkerPath + " " + outPath;
    std::string ssss2 = "VsDevCmd" + compilerComand + checkerPath + " " + outPath;

    std::string pathexe = "C:\\Windows\\system32\\cmd.exe";
    std::string command = "cmd /c notepad.exe";

    std::string pathexe1 = "C:\\lol2.bat";
    std::string command1 = "cmd /c notepad.exe";

    std::string sas = 
        "\"C:\\Program Files(x86)\\Microsoft\ Visual\ Studio\\2019\Community\\Common7\Tools\\VsDevCmd.bat\" & cl /EHsc \"D:\\hello.cpp\"";


    std::string sas2 =
        "C:\lol3.bat & cl D:\projects\VS_2019\ChineseTester\resources\task2\checker\checker.cpp /FoD:\projects\VS_2019\ChineseTester\resources\task2\checker\checker.exe
;
    
    std::cout << std::endl << ssss << std::endl;
    //process.create(makeBadString(sas), L"");
    process.create(makeBadString("C:\\lol2.bat"), makeBadString(ssss2));
    //process.create(makeBadString(compilerPath), makeBadString(ssss1));
    //process.create(makeBadString(pathexe), makeBadString(command));
     //process.create(makeBadString(pathexe1), makeBadString(command1));
    //process.create(makeBadString("C:\Windows\\system32\\cmd.exe"), makeBadString("cmd"));
    process.run();
    //WinExec(ssss.c_str(), SW_SHOWNORMAL);
    //SW_HIDE

    int cnt = 10e9;
    while (--cnt);
    std::cout << "--------------";

    std::wstring stupidChecker = getMainPath() + L"\ChineseTester\checker.exe";
    CopyFile(stupidChecker.c_str(), checkerEXE.c_str(), FALSE);
    //std::ifstream fromFile2(stupidChecker, std::ios::in | std::ios::binary);
    //std::ofstream toFile2(checkerEXE, std::ios::out | std::ios::binary);
   // std::copy(std::istreambuf_iterator<char>(fromFile2), std::istreambuf_iterator<char>(),
    //    std::ostreambuf_iterator<char>(toFile2));
    //char c;
//while (fromFile2 >> c) toFile2 << c;
    //CopyFile(szFilePath.c_str(), szCopyPath.c_str(), FALSE);

}

/*

C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat cl /EHsc U:\_Private\ChineseTester\ChineseTester\ChineseTester\resources\task2\checker\checker.cpp
*/


/*

#include <cstring>
#include <string>
#include <iostream>

#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>

std::wstring GetEnvString()
{
    wchar_t* env = GetEnvironmentStrings();
    if (!env)
        abort();
    const wchar_t* var = env;
    size_t totallen = 0;
    size_t len;
    while ((len = wcslen(var)) > 0)
    {
        totallen += len + 1;
        var += len + 1;
    }
    std::wstring result(env, totallen);
    FreeEnvironmentStrings(env);
    return result;
}

int main()
{
    std::wstring env = GetEnvString();
    env += L"myvar=boo";
    env.push_back('\0'); // somewhat awkward way to embed a null-terminator

    STARTUPINFO si = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION pi;

   // wchar_t cmdline[] = L"cmd.exe /C C:\\Desktop\\MyBatFile.bat";
    //wchar_t cmdline[] =
    //    L"\"C:\\Program Files(x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\Tools\\VsDevCmd.bat\"";

    wchar_t cmdline[] = L"C:\\lol2.bat";
    if (!CreateProcess(NULL, cmdline, NULL, NULL, false, CREATE_UNICODE_ENVIRONMENT,
        (LPVOID)env.c_str(), NULL, &si, &pi))
    {
        std::cout << GetLastError();
        abort();
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}


*/



std::wstring makeComand(char aFileEnding)
{
    std::wstring curPath = getMainPath() + RESOURCES + L"task1\\";
    std::map<char, std::wstring> m =
    {
        { 'y', L"python " + curPath + L"solution\\plus.py" },
        { 'p', curPath + L"solution\\plus.exe" },
        { 'e',  curPath + L"solution\\plus.exe" }
    };
    std::wstring comand = m[aFileEnding];
    return comand;
}

void check(std::wstring aSolutionName, MDatabaseQuery& aDBQ)
{
    std::wstring curPath = getMainPath() + RESOURCES + L"task1\\";

    std::string resultSS;
    int testNum = 1;

    std::wstring comand;
    if (aSolutionName.back() == 'y')
    {
        comand = L"python " + curPath + L"solution\\plus.py";
    }
    else if (aSolutionName.back() == 'p')
    {
        // TODO: compilation
        comand = curPath + L"solution\\plus.exe";
    }
    else if (aSolutionName.back() == 'e')
    {
        // TODO: compilation
        comand = curPath + L"solution\\plus.exe";
    }

    std::pair<uint_64, uint_64> results = { 0,0 };
    for (; testNum <= 5; ++testNum)
    {
        Process code;
        std::wstring testAddress = curPath + L"tests\\" + std::to_wstring(testNum);
        std::wstring outAddress = curPath + L"output\\" + std::to_wstring(testNum);

        //
        code.IORedirection(testAddress, outAddress);
        //code.create(comand, L"");
        //code.create(L"", curPath + L"solution\\plus.exe",);
        code.create(L"", comand);
        std::pair<uint_64, uint_64> cur = code.run(0, 0);

        results.first = max(results.first, cur.first);
        results.second = max(results.second, cur.second);

        Process checker;
        std::wstring answerAddress = curPath + L"answer\\" + std::to_wstring(testNum);
        std::wstring resultAddress = curPath + L"result\\" + std::to_wstring(testNum);
        std::wstring parameters = L"sas input " + outAddress + L" " + answerAddress;
        checker.IORedirection(L"", resultAddress);
        checker.create(curPath + L"checker\\checker.exe", parameters);
        checker.run();

        std::ifstream resultFile(resultAddress);
        resultFile >> resultSS;
        if (resultSS != "ok")
        {
            //WD_ERROR(mainCheck.0, "Can't open file " + makeGoodString(aTaskPath) + "init");
            WD_LOG("Result not okay");
            break;
        }
    }
    aDBQ.writeResult(resultSS, results.first, results.second);

    WD_LOG("Final result : " + resultSS);
    WD_LOG("Final time : " + results.first);
    WD_LOG("Final memory : " + results.second);
}
#include "database_query.h"

int main()
{
    makeChecker(L"task2");
    return 0;
    std::wstring aTaskName = L"task2";
    std::wstring basePath = getMainPath() + RESOURCES + L"data_base.sqlite3";
    std::wstring taskPath = getMainPath() + RESOURCES + aTaskName + L"\\";
    int id;
    std::cin >> id;

    MDatabaseQuery DBQ(basePath, taskPath);
    DBQ.makeTestCatalog(id);
    check(L"plus.py", DBQ);
    return 0;


    //B();

    //std::wstring resoPath = getMainPath() + RESOURCES;

    std::wstring curPath = getMainPath() + RESOURCES + L"task1\\";

    std::string resultSS;
    int testNum = 1;
    //for (; testNum <= 5; ++testNum)
    //{
    //    Core core;
    //    //std::wstring testAddress = curPath + L"tests\\" + std::to_wstring(testNum);
    //    //std::wstring outAddress = curPath + L"output\\" + std::to_wstring(testNum);
    //    //core.runProcess(curPath + L"solution\\plus.exe", testAddress, outAddress);
    //    std::wstring testAddress = curPath + L"tests\\" + std::to_wstring(testNum);
    //    std::wstring outAddress = curPath + L"output\\" + std::to_wstring(testNum);
    //    std::wstring pasarg = curPath + L"solution\\plus.py";
    //    std::wstring parameters1 = L"python " + pasarg;
    //    //Run run1;
    //    //run1.runProcess(L"", testAddress, outAddress, parameters1);
    //    core.runProcess(parameters1, testAddress, outAddress);

    //    Run run;
    //    std::wstring answerAddress = curPath + L"answer\\" + std::to_wstring(testNum);
    //    std::wstring resultAddress = curPath + L"result\\" + std::to_wstring(testNum);
    //    std::wstring parameters = L"sas input " + outAddress + L" " + answerAddress;

    //    std::wstring sss = curPath + L"checker\\checker.exe";
    //    run.runProcess(sss, L"", resultAddress, parameters);

    //    std::ifstream resultFile(resultAddress);
    //    resultFile >> resultSS;
    //    if (resultSS != "ok") break;
    //}
    //std::cout << resultSS << std::endl;

    //std::cout << std::endl << "---\n";

    //std::wstring path = L"D:\\projects\\VS_2019\\ChineseTester\\resources\\";
    //std::wstring programPath = path + L"MikeMirzayanov.exe";
    ////std::wstring programPath = path + L"ConsoleApplication1.exe";
    //std::wstring inputPath = path + L"input.txt";
    //std::wstring outputPath = path + L"output.txt";

wchar_t c1[] = 
L"sas "

L"D:\\projects\\VS_2019\\ChineseTester\\resources\\cinput.txt "

L"D:\\projects\\VS_2019\\ChineseTester\\resources\\coutput.txt "

L"D:\\projects\\VS_2019\\ChineseTester\\resources\\canswer.txt";


    std::wstring parameters = c1;
    //runProcess(programPath, inputPath, outputPath);

    

//    for (int i = 0; i < 3; ++i)
//    {
//        Core core;
//
//        //    Run run;
////    run.runProcess(programPath, inputPath, outputPath, parameters);
//    }
}
//
//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
//
//#include <unistd.h>
//#include <spawn.h>
//#include <sys/wait.h>
//
//extern char** environ;
//
//void test_fork_exec(void);
//void test_posix_spawn(void);
//
//int main(void) {
//    test_fork_exec();
//    test_posix_spawn();
//    return EXIT_SUCCESS;
//}
//
//void test_fork_exec(void) {
//    pid_t pid;
//    int status;
//    puts("Testing fork/exec");
//    fflush(NULL);
//    pid = fork();
//    switch (pid) {
//    case -1:
//        perror("fork");
//        break;
//    case 0:
//        execl("/bin/ls", "ls", (char*)0);
//        perror("exec");
//        break;
//    default:
//        printf("Child id: %i\n", pid);
//        fflush(NULL);
//        if (waitpid(pid, &status, 0) != -1) {
//            printf("Child exited with status %i\n", status);
//        }
//        else {
//            perror("waitpid");
//        }
//        break;
//    }
//}
//
//void test_posix_spawn(void) {
//    pid_t pid;
//    char* argv[] = { "ls", (char*)0 };
//    int status;
//    puts("Testing posix_spawn");
//    fflush(NULL);
//    status = posix_spawn(&pid, "/bin/ls", NULL, NULL, argv, environ);
//    if (status == 0) {
//        printf("Child id: %i\n", pid);
//        fflush(NULL);
//        if (waitpid(pid, &status, 0) != -1) {
//            printf("Child exited with status %i\n", status);
//        }
//        else {
//            perror("waitpid");
//        }
//    }
//    else {
//        printf("posix_spawn: %s\n", strerror(status));
//    }
//}