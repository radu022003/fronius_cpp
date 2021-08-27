#pragma once
#include "Poco/Data/Session.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/LocalDateTime.h"
#include "Poco/DateTime.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"
#include <iostream>
#include <fronius.h>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

class SqliteConn
{
private:
    /* data */
    std::unique_ptr<Poco::Data::Session> m_session;
    SqliteConn(std::unique_ptr<Poco::Data::Session> session);
    const std::string currentDateTime();
public:
    static SqliteConn *create(std::string const &fileName);
    void insert(PowerFlow &data);
    ~SqliteConn();
};
