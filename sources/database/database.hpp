#ifndef DATABASE_HPP
#define DATABASE_HPP

//--------------------------------------------------------------------------------

#include <string>

#include "sqlite_database.hpp"
#include "postgresql.hpp"

//--------------------------------------------------------------------------------

namespace data
{
    #if   defined(SQLITE)
    class Database : public SQLiteDatabase
    #elif defined(POSTGRESQL)
    class Database : public Posdtgres
    #endif
    {
    public:
        Database(std::string aDBName) noexcept;
        virtual ~Database() = default;

        Database(const Database& other) = delete;
        Database& operator=(const Database& other) = delete;

        Database(Database&& other) = delete;
        Database& operator=(Database&& other) = delete;
    };
}

//--------------------------------------------------------------------------------

#endif // !DATABASE_HPP
