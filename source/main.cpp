#include <string>

#include "core.h"
#include "database.h"

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

    if (sqlite3_open("D:\\projects\\VS 2019\\ChineseTester\\resources\\db.sqlite3", &ppDb)
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
    int num;
    //std::cin >> num;

    Database base("D:\\projects\\VS 2019\\ChineseTester\\resources\\data_base.sqlite3");
    //base.select("core_solution", "file_name, contest_id", "id = " + std::to_string(num));
    base.select("core_solutions", "", "");

    //int n = *(static_cast<const int*> (base.getNextRow(1)));
    base.step();
    //for(int i = 0; i < 4; ++i)
    //const unsigned char* ss = base.sqlite3_column_int64(4);
    int ss = base.getInt64FromRow(4);
    cout << ss;
    
    //for (auto& i : ss) cout << i;
    //for (int i = 0; ss[i] != 0; ++i) cout << i;
    //cout << n;
    //cout << (std::string) base.getNextRow(1);

    std::wstring path = L"D:\\projects\\VS\\Tester\\Tester\\Resources\\";
    std::wstring programPath = path + L"NeoTest2.exe";
    std::wstring inputPath = path + L"input";
    std::wstring outputPath = path + L"output.b";

    Core core;
    //core.runProcess(programPath, inputPath, outputPath);
}

int main()
{
    cout << "A: ";
    A();
    cout << "\nB: ";
    B();
}