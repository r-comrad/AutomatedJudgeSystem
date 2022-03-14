//#ifndef POSTGRES_HPP
//#define POSTGRES_HPP
//
//#include <libpq-fe.h>
//
//class Posdtgres
//{
//public:
//    Posdtgres();
//    std::shared_ptr<PGconn> connection() const;
//
//private:
//    void establish_connection();
//
//    std::string m_dbhost = "localhost";
//    int         m_dbport = 5432;
//    std::string m_dbname = "demo";
//    std::string m_dbuser = "postgres";
//    std::string m_dbpass = "postgres";
//
//    std::shared_ptr<PGconn>  m_connection;
//
//};
//
//#endif // !POSTGRES_HPP
