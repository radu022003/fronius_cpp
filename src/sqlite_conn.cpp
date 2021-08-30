#include <sqlite_conn.h>
#include <chrono>
#include <ctime> 

SqliteConn::SqliteConn(std::unique_ptr<Poco::Data::Session> session) : m_session(std::move(session))
{
    // (re)create table
    *m_session
        << "CREATE TABLE IF NOT EXISTS Fronius (id INTEGER PRIMARY KEY AUTOINCREMENT, time DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP, Load REAL, Grid REAL, PV REAL, Day REAL, Total REAL, Year REAL)",
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

const std::string SqliteConn::currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

    return buf;
}

void SqliteConn::insert(PowerFlow &data)
{
    //Poco::Data::Session session("SQLite", "sample.db");
    cout << "prepare data for insertion" << endl;
    Statement insert(*m_session);
    insert << "INSERT OR REPLACE INTO Fronius VALUES(NULL, NULL, ?, ?, ?, ?, ?, ?)",
        use(data.dP_Load),
        use(data.dP_Grid),
        use(data.dP_PV),
        use(data.iE_Day),
        use(data.dE_Total),
        use(data.dE_Year),
        now;
}