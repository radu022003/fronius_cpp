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
#include <memory>

using namespace Poco::Net;
using namespace Poco;
using namespace std;

class FroniusClient
{
private:
    /* data */
    std::shared_ptr<HTTPClientSession> m_session;
    HTTPRequest m_req;
    HTTPResponse m_res;
    URI m_uri;
    std::string m_apiVersion{};

    FroniusClient(std::shared_ptr<HTTPClientSession> session);

public:
    static FroniusClient *create(std::shared_ptr<YAML::Node> parentNode, std::shared_ptr<HTTPClientSession> session);
    void getApiVersion();
    const std::string getHost()
    {
        return m_session->getHost();
    };
    const uint16_t getPort()
    {
        return m_session->getPort();
    };

    ~FroniusClient();
};
