//#ifndef POSTGRES_HPP
//#define POSTGRES_HPP
//
//#include <libpq-fe.h>
//
//#include "domain/my_strings.hpp"
//
//class Posdtgres
//{
//public:
//    Posdtgres();
//    std::shared_ptr<Posdtgres> connection() const;
//
//private:
//    void establish_connection();
//
//    str_val m_dbhost = "localhost";
//    int         m_dbport = 5432;
//    str_val m_dbname = "test_sys";
//    str_val m_dbuser = "sys_user";
//    str_val m_dbpass = "1209root";
//
//    std::shared_ptr<PGconn>  m_connection;
//
//};
//
//#endif // !POSTGRES_HPP
