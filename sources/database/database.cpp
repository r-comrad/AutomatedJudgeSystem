#include "database.hpp"

//--------------------------------------------------------------------------------

data::Database::Database(std::string aDBName) noexcept
    #ifdef SQLITE
    :
    SQLiteDatabase(aDBName)
    #endif
{}

//--------------------------------------------------------------------------------
