#include "database.hpp"

//--------------------------------------------------------------------------------

data::Database::Database(std::string aDBName) noexcept :
    SQLiteDatabase(aDBName)
{}

//--------------------------------------------------------------------------------
