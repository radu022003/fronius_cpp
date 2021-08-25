#include <sqlite_conn.h>

SqliteConn::SqliteConn(std::unique_ptr<Poco::Data::Session> session) : m_session(std::move(session))
{
    // drop sample table, if it exists
    //m_session << "DROP TABLE IF EXISTS Person", now;

    // (re)create table
    /*
    *m_session
        << "CREATE TABLE Person (Name VARCHAR(30), Address VARCHAR, Age INTEGER(3))",
        now;
    */
}

SqliteConn::~SqliteConn()
{
}

SqliteConn *SqliteConn::create(std::unique_ptr<Poco::Data::Session> session)
{
    // register SQLite connector
    Poco::Data::SQLite::Connector::registerConnector();
    return new SqliteConn(std::move(session));
}

void SqliteConn::insert(const PowerFlow &data)
{
}