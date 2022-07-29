#include "database/sqlite_database.hpp"

//--------------------------------------------------------------------------------
//                        SQLLITE INTERFACE  IMPLEMENTATION
//--------------------------------------------------------------------------------

SQLiteDatabase::SQLiteDatabase(const std::string& aPath) :
    mBase(NULL)
{
    WRITE_LOG("Opening_database:", aPath);
    if (sqlite3_open(aPath.c_str(), &mBase) != SQLITE_OK) 
        WRITE_ERROR("SQLiteDatabase", "constructor(string)", 0, "Can't_open_database ", aPath);
    //TODO: check don't work
}

SQLiteDatabase::~SQLiteDatabase() {}


// void 
// SQLiteDatabase::select
// (
//     dom::String&&       aTableName,
//     dom::String&&       aColum,
//     dom::String&&       aConditon,
//     int                 aStatementID
// )

// {
//     if (aColum == "") aColum = "*";
//     if (aConditon != "") aConditon.pushFront(" WHERE "); //= " WHERE " + std::move(aConditon);
//     dom::String statement = "SELECT " + std::move(aColum);
//     statement += " FROM " + std::move(aTableName);
//     statement += std::move(aConditon);

//     prepare(std::move(statement), aStatementID);
// }
void 
SQLiteDatabase::select
(
    std::string&&       aTableName,
    std::string&&       aColum,
    std::string&&       aConditon,
    int                 aStatementID
) noexcept
{
    if (aColum == "") aColum = "*";
    if (aConditon != "") aConditon = " WHERE " + std::move(aConditon);
    std::string statement = "SELECT " + std::move(aColum) +
        " FROM " + std::move(aTableName) + std::move(aConditon);

    prepare(std::move(statement), aStatementID);
}

void 
SQLiteDatabase::update
(
    std::string&&       aTableName,
    std::string&&       aValue,
    std::string&&       aConditon,
    int                 aStatementID
) noexcept
{
    std::string statement = "UPDATE " + std::move(aTableName) +
        " SET " + std::move(aValue) +
        " WHERE " + std::move(aConditon);

    prepare(std::move(statement), aStatementID);
}

std::optional<dom::CharArray>
SQLiteDatabase::getTextFromRow(int aColumNumber, int aStatementID)
{
    std::optional<dom::CharArray> result = {};
    auto ptr = sqlite3_column_text(mStatement[aStatementID], aColumNumber);
    if (ptr != nullptr) result = dom::CharArray(ptr);
    return result;
}


dom::CharArray
SQLiteDatabase::getText16FromRow(int aColumNumber, int aStatementID)
{
    return dom::CharArray(sqlite3_column_text16(mStatement[aStatementID], aColumNumber));
}

int 
SQLiteDatabase::getIntFromRow(int aColumNumber, int aStatementID)
{
    return sqlite3_column_int(mStatement[aStatementID], aColumNumber);
}

int64_t
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

// dom::String
// SQLiteDatabase::toAscii(dom::String s)
// {
//     //TODO: use my defines
//     int cnt = 0;
//     while (s[cnt++]);
//     char* result = (char*) malloc(sizeof(char) * (cnt));
//     result[cnt - 1] = 0;
//     for (int i = 0; s[i];) result[i] = s[i++];
//     return result;
//     //TODO end
// }

void 
SQLiteDatabase::prepare(std::string&& aStatment, int aStatementID)
{
    if (mStatement.size() < aStatementID + 1)
    {
        mStatement.resize(aStatementID + 1);
    }
    mStatement[aStatementID] = nullptr;

    if (sqlite3_prepare_v2(
        mBase,                          /* Database handle */
        aStatment.c_str(),              /* SQL statement, UTF-8 encoded */
        -1,                             /* Maximum length of zSql in bytes. */
        &(mStatement[aStatementID]),    /* OUT: Statement handle */
        nullptr                         /* OUT: Pointer to unused portion of zSql */
    ) != SQLITE_OK) WRITE_ERROR("SQLiteDatabase", "prepare", 10, "Can't_prepare_statement", aStatment);
}