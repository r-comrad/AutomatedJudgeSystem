#include "database/sqlite_database.hpp"

//--------------------------------------------------------------------------------

data::SQLiteDatabase::SQLiteDatabase(const std::string& aPath) :
    mBase(NULL)
{
    WRITE_LOG("Opening_database:", aPath);
    if (sqlite3_open(aPath.c_str(), &mBase) != SQLITE_OK) 
        WRITE_ERROR("SQLiteDatabase", "constructor(string)", 0, "Can't_open_database ", aPath);
    //TODO: check don't work
}

//--------------------------------------------------------------------------------

data::SQLiteDatabase::~SQLiteDatabase() {}

//--------------------------------------------------------------------------------

void 
data::SQLiteDatabase::select
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

//--------------------------------------------------------------------------------

void 
data::SQLiteDatabase::update
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

//--------------------------------------------------------------------------------

std::optional<dom::CharArray>
data::SQLiteDatabase::getTextFromRow(int aColumNumber, int aStatementID)
{
    std::optional<dom::CharArray> result = {};
    auto ptr = sqlite3_column_text(mStatement[aStatementID], aColumNumber);
    if (ptr != nullptr) 
    {
        result = dom::CharArray(ptr);
    }
    return result;
}

//--------------------------------------------------------------------------------

dom::CharArray
data::SQLiteDatabase::getText16FromRow(int aColumNumber, int aStatementID)
{
    return dom::CharArray(sqlite3_column_text16(mStatement[aStatementID], aColumNumber));
}

//--------------------------------------------------------------------------------

int 
data::SQLiteDatabase::getIntFromRow(int aColumNumber, int aStatementID)
{
    return sqlite3_column_int(mStatement[aStatementID], aColumNumber);
}

//--------------------------------------------------------------------------------

int64_t
data::SQLiteDatabase::getInt64FromRow(int aColumNumber, int aStatementID)
{
    return sqlite3_column_int64(mStatement[aStatementID], aColumNumber);
}

//--------------------------------------------------------------------------------

void 
data::SQLiteDatabase::closeStatment(int aStatementID)
{
    sqlite3_finalize(mStatement[aStatementID]);
    mStatement[aStatementID] = NULL;
    while (mStatement.size() > 1 && 
        mStatement[aStatementID] == NULL) mStatement.pop_back();
}

//--------------------------------------------------------------------------------

int 
data::SQLiteDatabase::step(int aStatementID)
{
    return sqlite3_step(mStatement[aStatementID]);
}

//--------------------------------------------------------------------------------

void 
data::SQLiteDatabase::prepare(std::string&& aStatment, int aStatementID)
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

//--------------------------------------------------------------------------------
