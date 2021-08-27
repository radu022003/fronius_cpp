#include <sqlite_conn.h>

SqliteConn::SqliteConn(std::unique_ptr<Poco::Data::Session> session) : m_session(std::move(session))
{
    // drop sample table, if it exists
    //m_session << "DROP TABLE IF EXISTS Person", now;

    // (re)create table
    *m_session
        << "CREATE TABLE IF NOT EXISTS Fronius (id INTEGER PRIMARY KEY AUTOINCREMENT, time TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP, Load REAL, Grid REAL, PV REAL, Day REAL, Total REAL, Year REAL)",
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
    //Poco::Data::Session session("SQLite", "sample.db");
    Poco::Timestamp ts;
    Poco::DateTime dt(ts);
    Poco::LocalDateTime ldt(dt);
    std::string current_time = Poco::DateTimeFormatter::format(ldt, Poco::DateTimeFormat::SORTABLE_FORMAT);
    cout << current_time << endl;
    Statement insert(*m_session);
    insert << "INSERT INTO Fronius VALUES(NULL, ?, ?, ?, ?, ?, ?, ?)",
        use(current_time),
        use(data.dP_Load),
        use(data.dP_Grid),
        use(data.dP_PV),
        use(data.iE_Day),
        use(data.dE_Total),
        use(data.dE_Year),
        now;
}