#include <sqlite_conn.h>
#include <Poco/Data/SQLite/SQLiteException.h>
#include <chrono>
#include <ctime>

SqliteConn::SqliteConn(std::unique_ptr<Poco::Data::Session> session) : m_session(std::move(session))
{
    // drop sample table, if it exists
    //m_session << "DROP TABLE IF EXISTS Person", now;

    // (re)create table
    *m_session
        << "CREATE TABLE IF NOT EXISTS Fronius (id INTEGER PRIMARY KEY AUTOINCREMENT, time DATETIME DEFAULT CURRENT_TIMESTAMP, Load REAL, Grid REAL, PV REAL, Day INTEGER, Total REAL, Year REAL)",
        now;
}

SqliteConn::~SqliteConn()
{
}

SqliteConn *SqliteConn::create(std::string const &fileName)
{
    // register SQLite connector
    Poco::Data::SQLite::Connector::registerConnector();
    auto session = std::make_unique<Poco::Data::Session>("SQLite", fileName);
    return new SqliteConn(std::move(session));
}


void SqliteConn::insert(PowerFlow &data)
{
    Statement insert(*m_session);
    try
    {
        insert << "INSERT OR REPLACE INTO Fronius VALUES(NULL, NULL, ?, ?, ?, ?, ?, ?)",
            use(data.dP_Load),
            use(data.dP_Grid),
            use(data.dP_PV),
            use(data.iE_Day),
            use(data.dE_Total),
            use(data.dE_Year),
            now;
    }
    catch (const Poco::Data::SQLite::InvalidSQLStatementException &e)
    {

        std::cerr << e.what() << '\n';
    }
}