#pragma once
#include "Poco/Data/Session.h"
#include "Poco/Data/SQLite/Connector.h"
#include <iostream>
#include <fronius.h>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

struct Data
{
    /* data */
};

class SqliteConn
{
private:
    /* data */
    std::unique_ptr<Poco::Data::Session> m_session;
    SqliteConn(std::unique_ptr<Poco::Data::Session> session);

public:
    static SqliteConn *create(std::unique_ptr<Poco::Data::Session> session);
    void insert(const PowerFlow &data);
    ~SqliteConn();
};
