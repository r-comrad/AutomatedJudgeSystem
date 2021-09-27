#include "database.h"

Database::Database(std::string aPath) :
    mBase(NULL)
{
    if (sqlite3_open(aPath.c_str(), &mBase)
    //const char* Bb;
    //if (sqlite3_open_v2(aPath.c_str(), &mBase, SQLITE_OPEN_READWRITE, Bb)
        != SQLITE_OK)
    {
        std::cout << "ERROR: can't open database";
    }
}

void Database::select(std::string aTableName, std::string aColum, std::string aConditon)
{
    mStatement = NULL;
    if (aColum == "") aColum = "*";
    if (aConditon != "") aConditon = " WHERE " + aConditon;
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

const unsigned char* Database::getTextFromRow(int aColumNumber)
{
    return sqlite3_column_text(mStatement, aColumNumber);
}

int Database::getIntFromRow(int aColumNumber)
{
    return sqlite3_column_int(mStatement, aColumNumber);
}

sint_64 Database::getInt64FromRow(int aColumNumber)
{
    return sqlite3_column_int64(mStatement, aColumNumber);
}

void Database::closeStatment()
{
    sqlite3_finalize(mStatement);
}
//
//int Database::takeProblemId(sqlite3* ppDb)
//{
//    sqlite3_stmt* ppStmt = NULL;
//    sqlite3_prepare_v2(
//        ppDb,              /* Database handle */
//        "SELECT * FROM core_solutions;",             /* SQL statement, UTF-8 encoded */
//        -1,                 /* Maximum length of zSql in bytes. */
//        &ppStmt,             /* OUT: Statement handle */
//        NULL                /* OUT: Pointer to unused portion of zSql */
//    );
//
//    sqlite3_step(ppStmt);
//    int result = sqlite3_column_int64(ppStmt, 4);
//    sqlite3_finalize(ppStmt);
//    return result;
//}

void Database::step()
{
    sqlite3_step(mStatement);
}