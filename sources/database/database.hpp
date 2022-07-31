#ifndef DATABASE_HPP
#define DATABASE_HPP

//--------------------------------------------------------------------------------

#include <string>

#include "sqlite_database.hpp"
#include "postgresql.hpp"

//--------------------------------------------------------------------------------

namespace data
{
    class Database : public SQLiteDatabase
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
