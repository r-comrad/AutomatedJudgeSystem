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
    std::string aConditon,
    int aStatementID
)
{
    if (aColum == "") aColum = "*";
    if (aConditon != "") aConditon = " WHERE " + aConditon;
    std::string statement = "SELECT " + aColum + " FROM " + aTableName + aConditon;
    prepare(statement, aStatementID);
}

void 
Database::update
(
    std::string aTableName,
    std::string aValue,
    std::string aConditon,
    int aStatementID
)
{
    std::string statement = "UPDATE " + aTableName + " SET " + aValue + " WHERE " + aConditon;
    prepare(statement, aStatementID);
}

const unsigned char* 
Database::getTextFromRow(int aColumNumber, int aStatementID)
{
    return sqlite3_column_text(mStatement[aStatementID], aColumNumber);
}


const void* 
Database::getText16FromRow(int aColumNumber, int aStatementID)
{
    return sqlite3_column_text16(mStatement[aStatementID], aColumNumber);
}

int 
Database::getIntFromRow(int aColumNumber, int aStatementID)
{
    return sqlite3_column_int(mStatement[aStatementID], aColumNumber);
}

sint_64 
Database::getInt64FromRow(int aColumNumber, int aStatementID)
{
    return sqlite3_column_int64(mStatement[aStatementID], aColumNumber);
}

void 
Database::closeStatment(int aStatementID)
{
    sqlite3_finalize(mStatement[aStatementID]);
    mStatement[aStatementID] = NULL;
    while (mStatement.size() > 1 && 
        mStatement[aStatementID] == NULL) mStatement.pop_back();
}

int 
Database::step(int aStatementID)
{
    return sqlite3_step(mStatement[aStatementID]);
}

char* 
Database::toAscii(const unsigned char* s, int aStatementID)
{
    int cnt = 0;
    while (s[cnt++]);
    char* result = (char*) malloc(sizeof(char) * (cnt));
    result[cnt - 1] = 0;
    for (int i = 0; s[i];) result[i] = s[i++];
    return result;
}

void 
Database::prepare(std::string& aStatment, int aStatementID)
{
    if (mStatement.size() < aStatementID + 1)
    {
        mStatement.resize(aStatementID + 1);
    }
    mStatement[aStatementID] = NULL;

    if (sqlite3_prepare_v2(
        mBase,              /* Database handle */
        aStatment.c_str(),             /* SQL statement, UTF-8 encoded */
        -1,                 /* Maximum length of zSql in bytes. */
        &(mStatement[aStatementID]),             /* OUT: Statement handle */
        NULL                /* OUT: Pointer to unused portion of zSql */
    ) != SQLITE_OK) WD_ERROR(database.2, "Can't prepare statement " + aStatment);
}