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

void B()
{
    //int num;
    //std::cin >> num;

    //Database base("D:\\projects\\VS 2019\\ChineseTester\\resources\\data_base.sqlite3");
    Database base("U:\\_Private\\ChineseTester\\ChineseTester\\resources\\data_base.sqlite3");
    base.select("core_solutions", "file_name, contest_id", "id = " + std::to_string(10));
    //base.select("core_solutions", "", "");


    //contest

    //int n = *(static_cast<const int*> (base.getNextRow(1)));
    base.step();
    //for(int i = 0; i < 4; ++i)
    //const unsigned char* ss = base.sqlite3_column_int64(4);
    //int ss = base.getInt64FromRow(4);
    //int ss = base.getInt64FromRow(0);
    //cout << ss << " ";
    const unsigned char* fileName =  base.getTextFromRow(0);
    cout << "\nFile name: ";
    for (int i = 0; fileName[i];) cout << fileName[i++];

    cout << "\n";
    char* sss = base.toAscii(fileName);
    for (int i = 0; sss[i];) cout << sss[i++];

    int contest_id = base.getInt64FromRow(1);
    cout << "\nID: ";
    cout << " " << contest_id << "\n";
    base.closeStatment();

    //for (auto& i : ss) cout << i;
    //for (int i = 0; ss[i] != 0; ++i) cout << i;
    //cout << n;
    //cout << (std::string) base.getNextRow(1);

    base.select("core_contests", "time_limit, memory_limit", "id = " + std::to_string(contest_id));
    base.step();
    int time_limit = base.getInt64FromRow(0);
    int memory_limit = base.getInt64FromRow(1);
    cout << "\nTime limit: ";
    cout << time_limit;
    cout << "\nMemory limit: "<< memory_limit << "\n";
    base.closeStatment();
    //std::wstring path = L"D:\\projects\\VS\\Tester\\Tester\\Resources\\";
    //std::wstring programPath = path + L"NeoTest2.exe";
    //std::wstring inputPath = path + L"input";
    //std::wstring outputPath = path + L"output.b";



    base.select("core_test", "input, output", "contest_id = " + std::to_string(contest_id));
    base.step();
    const unsigned char* input = base.getTextFromRow(0);
    input[0];
    input[1];
    input[2];
    const unsigned char* output = base.getTextFromRow(1);
    for (int i = 0; input[i];) cout << input[i++];
    cout << "\n";
    for (int i = 0; output[i];) cout << output[i++];
    cout << "\n";

    base.step();
    for (int i = 0; input[i];) cout << input[i++];
    cout << "\n";
    for (int i = 0; output[i];) cout << output[i++];
    cout << "\n";
    base.closeStatment();

    Core core;
    //core.runProcess(programPath, inputPath, outputPath);

    //SELECT input, output FROM core_test WHERE contest_id = 4
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

#define RESOURCES L"resources\\"

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

void makeTestCatalog()
{

}

int main()
{
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

