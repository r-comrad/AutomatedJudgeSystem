#include "database.h"

Database::Database
(
    std::wstring aPath
) :
    mBase(NULL)
{
    WD_LOG("Opening database : " << makeGoodString(aPath));
    if (sqlite3_open(makeGoodString(aPath).c_str(), &mBase) != SQLITE_OK) 
        WD_ERROR(database.0, "Can't open database " + makeGoodString(aPath));
    //TODO: check don't work
    WD_END_LOG;
}

void 
Database::select
(
    std::string aTableName,
    std::string aColum, 
    std::string aConditon
)
{
    mStatement = NULL;
    if (aColum == "") aColum = "*";
    if (aConditon != "") aConditon = " WHERE " + aConditon;
    std::string statement = "SELECT " + aColum + " FROM " + aTableName + aConditon;
    prepare(statement);
}

void 
Database::update
(
    std::string aTableName,
    std::string aValue,
    std::string aConditon
)
{
    std::string statement = "UPDATE " + aTableName + " SET " + aValue + " WHERE " + aConditon;
    prepare(statement);
}

const unsigned char* 
Database::getTextFromRow(int aColumNumber)
{
    return sqlite3_column_text(mStatement, aColumNumber);
}


const void* 
Database::getText16FromRow(int aColumNumber)
{
    return sqlite3_column_text16(mStatement, aColumNumber);
}

int 
Database::getIntFromRow(int aColumNumber)
{
    return sqlite3_column_int(mStatement, aColumNumber);
}

sint_64 
Database::getInt64FromRow(int aColumNumber)
{
    return sqlite3_column_int64(mStatement, aColumNumber);
}

void 
Database::closeStatment()
{
    sqlite3_finalize(mStatement);
}

int 
Database::step()
{
    return sqlite3_step(mStatement);
}

char* 
Database::toAscii(const unsigned char* s)
{
    int cnt = 0;
    while (s[cnt++]);
    char* result = (char*) malloc(sizeof(char) * (cnt));
    result[cnt - 1] = 0;
    for (int i = 0; s[i];) result[i] = s[i++];
    return result;
}

void 
Database::prepare(std::string& aStatment)
{
    if (sqlite3_prepare_v2(
        mBase,              /* Database handle */
        aStatment.c_str(),             /* SQL statement, UTF-8 encoded */
        -1,                 /* Maximum length of zSql in bytes. */
        &mStatement,             /* OUT: Statement handle */
        NULL                /* OUT: Pointer to unused portion of zSql */
    ) != SQLITE_OK) WD_ERROR(database.2, "Can't prepare statement " + aStatment);
}