// Fronius.h
#pragma once
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/Path.h>
#include <Poco/URI.h>
#include <Poco/Exception.h>
#include <iostream>
#include <string>
#include <yaml-cpp/yaml.h>

using namespace Poco::Net;
using namespace Poco;
using namespace std;

class FroniusClient
{
private:
    /* data */
    HTTPClientSession m_session;
    HTTPRequest m_req;
    HTTPResponse m_res;
    URI m_uri;

    FroniusClient(std::shared_ptr<YAML::Node> parentNode);

public:
    static FroniusClient *create(std::shared_ptr<YAML::Node> parentNode);
    ~FroniusClient();
};
