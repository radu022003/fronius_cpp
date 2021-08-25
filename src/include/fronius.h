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
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/error/en.h>
#include <rapidjson/pointer.h>

using namespace Poco::Net;
using namespace Poco;
using namespace std;
using namespace rapidjson;

struct PowerFlow
{
    double dP_Load;
    double dP_Grid;
    double dP_PV;
    int iE_Day;
    double dE_Total;
    double dE_Year;

    friend ostream &operator<<(ostream &output, const PowerFlow &P)
    {
        output << "Load : " << P.dP_Load << endl;
        output << "Grid : " << P.dP_Grid << endl;
        output << "PV : " << P.dP_PV << endl;
        output << "Day : " << P.iE_Day << endl;
        output << "Year : " << P.dE_Year << endl;
        output << "Total : " << P.dE_Total << endl;
        return output;
    }
};

class FroniusClient
{
private:
    /* data */
    std::shared_ptr<HTTPClientSession> m_session;
    HTTPRequest m_req;
    HTTPResponse m_res;
    URI m_uri;
    int m_apiVersion{};
    std::string m_baseURL{};

    PowerFlow m_powerFlow;

    FroniusClient(std::shared_ptr<HTTPClientSession> session);

public:
    static FroniusClient *create(std::shared_ptr<YAML::Node> parentNode, std::shared_ptr<HTTPClientSession> session);
    void getApiVersion();
    void getPowerFlow();
    const std::string getHost()
    {
        return m_session->getHost();
    };
    const uint16_t getPort()
    {
        return m_session->getPort();
    };

    std::string &getBaseURL()
    {
        return m_baseURL;
    }

    int &getApiVersionNumber()
    {
        return m_apiVersion;
    }

    const PowerFlow &getFlowPowerData()
    {
        return m_powerFlow;
    }

    ~FroniusClient();
};
