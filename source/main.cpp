#include "database.h"
#include "run.h"
#include "database_query.h"

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

    //C:\Windows\SysWOW64\WindowsPowerShell\v1.0\\powershell.exe

    Process process;
    //std::wstring answerAddress = curPath + L"answer\\" + std::to_wstring(testNum);
    //std::wstring resultAddress = curPath + L"result\\" + std::to_wstring(testNum);
    //std::wstring parameters = L"sas input " + outAddress + L" " + answerAddress;


        //C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\Launch-VsDevShell.ps1
      //  run.runProcess( L"C:\\Windows\\SysWOW64\\WindowsPowerShell\\v1.0\\powershell.exe",
        //run.runProcess( L"C:\\VsDevCmd2.bat",
      //      L"", L"", L"cd " + checkerkath + L"\n" + L"cl /EHsc checker.cpp");
       //     L"", L"", L"C:\\Windows\\SysWOW64\\WindowsPowerShell\\v1.0\\powershell.exe cl /EHsc checker.cpp");


    //run.runProcess(L"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\Tools\\VsDevCmd.bat & cl /EHsc " + sss, L"", L"", L"");
    
    
    
    //std::string ssss = "\"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\Tools\\VsDevCmd.bat\" & cl /EHsc \""
    std::string ssss = "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\Tools\\VsDevCmd.bat cl /EHsc "
        + makeGoodString(sss)
        //+  " /LINK /OUT:\"" + getString(checkerEXE) + "\""
        ;
    std::cout << std::endl << ssss << std::endl;
    process.create(makeBadString(ssss), L"");
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
            break;
#ifdef _DBG_
            std::cout << "error";
#endif
        }
    }
    aDBQ.writeResult(resultSS, results.first, results.second);

    std::cout << "Final result : " << resultSS << std::endl;
    std::cout << "Final time : " << results.first << std::endl;
    std::cout << "Final memory : " << results.second << std::endl;
}
#include "database_query.h"
int main()
{
    makeChecker(L"task2");
    return 0;
    std::wstring aTaskName = L"task2";
    std::wstring basePath = getMainPath() + RESOURCES + L"data_base.sqlite3";
    std::wstring taskPath = getMainPath() + RESOURCES + aTaskName + L"\\";
    MDatabaseQuery DBQ(basePath, taskPath);
    DBQ.makeTestCatalog(10);
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

