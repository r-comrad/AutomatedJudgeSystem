#include <string>

#include "core.h"
#include "database.h"
#include "run.h"

int takeProblemId(sqlite3* ppDb)
{
    sqlite3_stmt* ppStmt = NULL;
    sqlite3_prepare_v2(
        ppDb,              /* Database handle */
        "SELECT * FROM core_solutions;",             /* SQL statement, UTF-8 encoded */
        -1,                 /* Maximum length of zSql in bytes. */
        &ppStmt,             /* OUT: Statement handle */
        NULL                /* OUT: Pointer to unused portion of zSql */
    );

    sqlite3_step(ppStmt);
    int result = sqlite3_column_int64(ppStmt, 4);
    sqlite3_finalize(ppStmt);
    return result;
}

int coutRows(sqlite3* ppDb, int problemNum)
{
    sqlite3_stmt* ppStmt = NULL;
    std::string ss = "SELECT COUNT(input) FROM core_test WHERE contest_id = " + std::to_string(problemNum) + ";";
    std::cout << ss << std::endl;
    sqlite3_prepare_v2(
        ppDb,              /* Database handle */
        ss.c_str(),             /* SQL statement, UTF-8 encoded */
        -1,                 /* Maximum length of zSql in bytes. */
        &ppStmt,             /* OUT: Statement handle */
        NULL                /* OUT: Pointer to unused portion of zSql */
    );
    sqlite3_step(ppStmt);
    int result = sqlite3_column_int(ppStmt, 0);
    return result;
}

void takeProblemId(sqlite3* ppDb, int problemNum)
{
    sqlite3_stmt* ppStmt = NULL;
    std::string ss = "SELECT input, output FROM core_test WHERE contest_id = " + std::to_string(problemNum) + ";";
    std::cout << ss << std::endl;
    sqlite3_prepare_v2(
        ppDb,              /* Database handle */
        ss.c_str(),             /* SQL statement, UTF-8 encoded */
        -1,                 /* Maximum length of zSql in bytes. */
        &ppStmt,             /* OUT: Statement handle */
        NULL                /* OUT: Pointer to unused portion of zSql */
    );
    sqlite3_step(ppStmt);
    auto result = sqlite3_column_text(ppStmt, 0);
    std::cout << result << std::endl;
    std::cout << sqlite3_column_text(ppStmt, 1) << std::endl;
    sqlite3_step(ppStmt);
    std::cout << sqlite3_column_text(ppStmt, 0) << std::endl;
    std::cout << sqlite3_column_text(ppStmt, 1) << std::endl;
    sqlite3_finalize(ppStmt);

    int i = 0;
    //for (; sqlite3_column_text(ppStmt, 0) != NULL; ++i) sqlite3_step(ppStmt);
    for (; sqlite3_column_bytes(ppStmt, 0) != 0; ++i)  sqlite3_step(ppStmt);;
    std::cout << i << std::endl;

    // return result;
}

void A()
{
    sqlite3* ppDb = NULL;

    if (sqlite3_open("U:\\_Private\\ChineseTester\\ChineseTester\\resources\\db.sqlite3", &ppDb)
        != SQLITE_OK)
    {
        std::cout << "ERROR: can't open database";
    }

    int result = takeProblemId(ppDb);
    std::cout << result << std::endl;
    //std::cout << coutRows(ppDb, result) << std::endl;
    //takeProblemId(ppDb, result);
    sqlite3_close(ppDb);
}


#include <vector>
#include <fstream>

std::wstring getMainPath() {
#ifdef WINDOWS_OSS
    TCHAR buffer[MAX_PATH] = { 0 };
    uint_8 size = GetModuleFileName(NULL, buffer, MAX_PATH);
    for (int i = 0; i < 2; ++i) while (buffer[--size] != L'\\');
    return std::wstring(buffer).substr(0, size + 1);
#endif
}

std::string getString(std::wstring aBadString)
{
    std::string goodString;
    for (auto& i : aBadString) goodString.push_back(char(i));
    return goodString;
}

#define RESOURCES L"resources\\"

void makeTestCatalog(int ID, std::wstring aTaskName)
{
    std::wstring taskath = getMainPath() + RESOURCES + aTaskName;
    std::ofstream taskFile(taskath + L"init");

    std::wstring basePath = getMainPath() + RESOURCES;
    Database base(getString(basePath) + "data_base.sqlite3");

    base.select("core_solutions", "file_name, contest_id", "id = " + std::to_string(ID));
    base.step();

    const unsigned char* fileName =  base.getTextFromRow(0);
    taskFile << "File_name: ";
    for (int i = 0; fileName[i];) taskFile << fileName[i++];

    int contest_id = base.getInt64FromRow(1);
    base.closeStatment();

    base.select("core_contests", "time_limit, memory_limit", "id = " + std::to_string(contest_id));
    base.step();
    int time_limit = base.getInt64FromRow(0);
    int memory_limit = base.getInt64FromRow(1);
    taskFile << "\nTime limit: " << time_limit;
    taskFile << "\nMemory limit: "<< memory_limit << "\n";
    base.closeStatment();

    base.select("core_test", "input, output", "contest_id = " + std::to_string(contest_id));
    std::wstring testAddress = taskath + L"tests\\";
    std::wstring ansAddress = taskath + L"answer\\";
    for(int i = 0; i < 1; ++i)
    {
        if (base.step() != 0) break;
        const unsigned char* input = base.getTextFromRow(0);
        const unsigned char* output = base.getTextFromRow(1);

        std::ofstream taskFile(testAddress + std::to_wstring(i));
        std::ofstream ansFile(testAddress + std::to_wstring(i));

        for (int i = 0; input[i];) taskFile << input[i++];
        for (int i = 0; output[i];) ansFile << output[i++];

        base.closeStatment();
    }
}

void check()
{
    std::wstring curPath = getMainPath() + RESOURCES + L"task1\\";

    std::string resultSS;
    int testNum = 1;
    for (; testNum <= 5; ++testNum)
    {
        Core core;
        std::wstring testAddress = curPath + L"tests\\" + std::to_wstring(testNum);
        std::wstring outAddress = curPath + L"output\\" + std::to_wstring(testNum);
        core.runProcess(curPath + L"plus.exe", testAddress, outAddress);

        Run run;
        std::wstring answerAddress = curPath + L"answer\\" + std::to_wstring(testNum);
        std::wstring resultAddress = curPath + L"result\\" + std::to_wstring(testNum);
        std::wstring parameters = L"sas input " + outAddress + L" " + answerAddress;
        run.runProcess(curPath + L"checker.exe", L"", resultAddress, parameters);

        std::ifstream resultFile(resultAddress);
        resultFile >> resultSS;
        if (resultSS != "ok") break;
    }
    cout << resultSS << endl;
}

int main()
{
    std::string
    while (std::cin >> ss);


    //B();

    //std::wstring resoPath = getMainPath() + RESOURCES;

    std::wstring curPath = getMainPath() + RESOURCES + L"task1\\";

    std::string resultSS;
    int testNum = 1;
    for (; testNum <= 5; ++testNum)
    {
        Core core;
        std::wstring testAddress = curPath + L"tests\\" + std::to_wstring(testNum);
        std::wstring outAddress = curPath + L"output\\" + std::to_wstring(testNum);
        core.runProcess(curPath + L"plus.exe", testAddress, outAddress);

        Run run;
        std::wstring answerAddress = curPath + L"answer\\" + std::to_wstring(testNum);
        std::wstring resultAddress = curPath + L"result\\" + std::to_wstring(testNum);
        std::wstring parameters = L"sas input " + outAddress + L" " + answerAddress;
        run.runProcess(curPath + L"checker.exe", L"", resultAddress, parameters);

        std::ifstream resultFile(resultAddress);
        resultFile >> resultSS;
        if (resultSS != "ok") break;
    }
    cout << resultSS << endl;

    cout << std::endl << "---\n";

    std::wstring path = L"D:\\projects\\VS_2019\\ChineseTester\\resources\\";
    std::wstring programPath = path + L"MikeMirzayanov.exe";
    //std::wstring programPath = path + L"ConsoleApplication1.exe";
    std::wstring inputPath = path + L"input.txt";
    std::wstring outputPath = path + L"output.txt";

wchar_t c1[] = 
L"sas "

L"D:\\projects\\VS_2019\\ChineseTester\\resources\\cinput.txt "

L"D:\\projects\\VS_2019\\ChineseTester\\resources\\coutput.txt "

L"D:\\projects\\VS_2019\\ChineseTester\\resources\\canswer.txt";


    std::wstring parameters = c1;
    //runProcess(programPath, inputPath, outputPath);

    Run run;
    

//    for (int i = 0; i < 3; ++i)
//    {
//        Core core;
//
//        //    Run run;
////    run.runProcess(programPath, inputPath, outputPath, parameters);
//    }
}

