#include "database/sqlite_database.hpp"

//--------------------------------------------------------------------------------
//					    SQLLITE INTERFACE  IMPLEMENTATION
//--------------------------------------------------------------------------------

SQLiteDatabase::SQLiteDatabase
(
    const std::string& aPath
) :
    mBase(NULL)
{
    WRITE_LOG("Opening_database:", aPath);
    if (sqlite3_open(aPath.c_str(), &mBase) != SQLITE_OK) 
        WRITE_ERROR("SQLiteDatabase", "constructor(string)", 0, "Can't_open_database ", aPath);
    //TODO: check don't work
}

SQLiteDatabase::~SQLiteDatabase() {}


void 
SQLiteDatabase::select
(
    const std::string&  aTableName,
    std::string         aColum,
    std::string         aConditon,
    int                 aStatementID
)

{
    if (aColum == "") aColum = "*";
    if (aConditon != "") aConditon = " WHERE " + aConditon;
    std::string statement = "SELECT " + aColum + " FROM " + aTableName + aConditon;
    prepare(statement, aStatementID);
}

void 
SQLiteDatabase::update
(
    const std::string&  aTableName,
    const std::string&  aValue,
    const std::string&  aConditon,
    int                 aStatementID
)
{
    std::string statement = "UPDATE " + aTableName + " SET " + aValue + " WHERE " + aConditon;
    prepare(statement, aStatementID);
}

const unsigned char* 
SQLiteDatabase::getTextFromRow(int aColumNumber, int aStatementID)
{
    return sqlite3_column_text(mStatement[aStatementID], aColumNumber);
}


const void* 
SQLiteDatabase::getText16FromRow(int aColumNumber, int aStatementID)
{
    return sqlite3_column_text16(mStatement[aStatementID], aColumNumber);
}

int 
SQLiteDatabase::getIntFromRow(int aColumNumber, int aStatementID)
{
    return sqlite3_column_int(mStatement[aStatementID], aColumNumber);
}

sint_64 
SQLiteDatabase::getInt64FromRow(int aColumNumber, int aStatementID)
{
    return sqlite3_column_int64(mStatement[aStatementID], aColumNumber);
}

void 
SQLiteDatabase::closeStatment(int aStatementID)
{
    sqlite3_finalize(mStatement[aStatementID]);
    mStatement[aStatementID] = NULL;
    while (mStatement.size() > 1 && 
        mStatement[aStatementID] == NULL) mStatement.pop_back();
}

int 
SQLiteDatabase::step(int aStatementID)
{
    return sqlite3_step(mStatement[aStatementID]);
}

char* 
SQLiteDatabase::toAscii(const unsigned char* s)
{
    //TODO: use my defines
    int cnt = 0;
    while (s[cnt++]);
    char* result = (char*) malloc(sizeof(char) * (cnt));
    result[cnt - 1] = 0;
    for (int i = 0; s[i];) result[i] = s[i++];
    return result;
    //TODO end
}

void 
SQLiteDatabase::prepare(const std::string& aStatment, int aStatementID)
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
    ) != SQLITE_OK) WRITE_ERROR("SQLiteDatabase", "prepare", 10, "Can't_prepare_statement", aStatment);
}