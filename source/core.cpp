#include "core.h"

#include "database.h"
#include "run.h"
#include "database_query.h"

#include <map>

void 
Core::compile(std::wstring aCheckerName, std::wstring aOutName, Language aLanguage)
{
    if (aLanguage == Language::MagicCPP)
    {
        Process process;
        //process.IORedirection(L"", L"");
        std::wstring comand = COMPILERS + L"\\magicCPPCompiler.cmd" + aCheckerName + aOutName;
        process.create(L"", comand);
        process.run();
    }
    for (int i = 0; i < 1e7; ++i);
}

void
Core::makeChecker(std::wstring aCheckerName)
{
    Core::Language language = getLanguage(aCheckerName);
    compile(aCheckerName, CHECKER_PATH, language);
}

Core::Language 
Core::getLanguage(std::wstring aFileName)
{
    int num = aFileName.find('.');
    if (num == -1) return Core::Language::NUN;
    
    Core::Language result = Core::Language::NUN;
    std::wstring fileExtension = aFileName.substr(num + 1);
    if (fileExtension == L"cpp") result = Core::Language::MagicCPP;
    if (fileExtension == L"py") result = Core::Language::Snake;
    return result;
}

void
Core::run()
{
    makeChecker(std::wstring aCheckerName);
}

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
    std::wstring aTaskName = L"task2";
    std::wstring basePath = getMainPath() + RESOURCES + L"data_base.sqlite3";
    std::wstring taskPath = getMainPath() + RESOURCES + aTaskName + L"\\";
    int id = 10;
    //std::cin >> id;
    makeChecker(L"task2");
    MDatabaseQuery DBQ(basePath, taskPath);
    DBQ.makeTestCatalog(id);
    check(L"plus.py", DBQ);
    return 0;


}