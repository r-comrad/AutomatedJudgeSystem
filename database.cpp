// ConsoleApplication1.cpp : ���� ���� �������� ������� "main". ����� ���������� � ������������� ���������� ���������.
//
/*
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
    //std::cout << ss << std::endl;
    sqlite3_prepare_v2(
        ppDb,              /* Database handle */
        ss.c_str(),             /* SQL statement, UTF-8 encoded */
        -1,                 /* Maximum length of zSql in bytes. */
        &ppStmt,             /* OUT: Statement handle */
        NULL                /* OUT: Pointer to unused portion of zSql */
    );
    sqlite3_step(ppStmt);
    auto result = sqlite3_column_text(ppStmt, 0);
    //std::cout << result << std::endl;
   // std::cout << sqlite3_column_text(ppStmt, 1) << std::endl;
    sqlite3_step(ppStmt);
    //std::cout << sqlite3_column_text(ppStmt, 0) << std::endl;
    //std::cout << sqlite3_column_text(ppStmt, 1) << std::endl;
    sqlite3_finalize(ppStmt);

    int i = 0;
    //for (; sqlite3_column_text(ppStmt, 0) != NULL; ++i) sqlite3_step(ppStmt);
    for (; sqlite3_column_bytes(ppStmt, 0) != 0; ++i)  sqlite3_step(ppStmt);;
    std::cout << i << std::endl;

    // return result;
}
*/

#include "database.h"

void Database::select(std::string aTableName, std::string aColum, std::string aConditon)
{
    mStatement = NULL;
    if (aColum == "") aColum = "*";
    std::string ss = "SELECT " + aColum + " FROM " + aTableName + aConditon;
    //std::cout << ss << std::endl;
    sqlite3_prepare_v2(
        mBase,              /* Database handle */
        ss.c_str(),             /* SQL statement, UTF-8 encoded */
        -1,                 /* Maximum length of zSql in bytes. */
        &mStatement,             /* OUT: Statement handle */
        NULL                /* OUT: Pointer to unused portion of zSql */
    );
}

auto Database::getNextRow(int aColumNumber)
{
    return sqlite3_column_text(mStatement, aColumNumber);
}

void Database::clouseStatment()
{
    sqlite3_finalize(mStatement);
}

    sqlite3_step(ppStmt);
    
    //std::cout << result << std::endl;
   // std::cout << sqlite3_column_text(ppStmt, 1) << std::endl;
    sqlite3_step(ppStmt);
    //std::cout << sqlite3_column_text(ppStmt, 0) << std::endl;
    //std::cout << sqlite3_column_text(ppStmt, 1) << std::endl;
    sqlite3_finalize(ppStmt);

    int i = 0;
    //for (; sqlite3_column_text(ppStmt, 0) != NULL; ++i) sqlite3_step(ppStmt);
    for (; sqlite3_column_bytes(ppStmt, 0) != 0; ++i)  sqlite3_step(ppStmt);;
    std::cout << i << std::endl;

    // return result;
}

Database::Database() :
    mBase       (NULL)
{
    if (sqlite3_open("D:\\projects\\VS\\\OHUENNIY_CHECKER\\Resources\\db.sqlite3", &mBase)
        != SQLITE_OK)
    {
        std::cout << "ERROR: can't open database";
    }
}



int Database::takeProblemId(sqlite3* ppDb)
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

int mainnn()
{
    sqlite3* ppDb = NULL;

    
    int result = takeProblemId(ppDb);
    std::cout << result << std::endl;
    //std::cout << coutRows(ppDb, result) << std::endl;
    takeProblemId(ppDb, result);
    return 0;
}

// ������ ���������: CTRL+F5 ��� ���� "�������" > "������ ��� �������"
// ������� ���������: F5 ��� ���� "�������" > "��������� �������"

// ������ �� ������ ������ 
//   1. � ���� ������������ ������� ����� ��������� ����� � ��������� ���.
//   2. � ���� Team Explorer ����� ������������ � ������� ���������� ��������.
//   3. � ���� "�������� ������" ����� ������������� �������� ������ ������ � ������ ���������.
//   4. � ���� "������ ������" ����� ������������� ������.
//   5. ��������������� �������� ������ ���� "������" > "�������� ����� �������", ����� ������� ����� ����, ��� "������" > "�������� ������������ �������", ����� �������� � ������ ������������ ����� ����.
//   6. ����� ����� ������� ���� ������ �����, �������� ������ ���� "����" > "�������" > "������" � �������� SLN-����.
