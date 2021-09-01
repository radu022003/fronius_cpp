// Fronius.h
#pragma once
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/Path.h>
#include <Poco/URI.h>
#include <Poco/Net/NetException.h>
#include <Poco/Exception.h>
#include <iostream>
#include <string>
#include <algorithm>
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

class PowerFlow
{
public:
    double dP_Load;
    double dP_Grid;
    double dP_PV;
    int iE_Day;
    double dE_Total;
    double dE_Year;

    friend ostream &operator<<(ostream &output, const PowerFlow &P)
    {
        output << "Load : " << std::to_string(P.dP_Load) << endl;
        output << "Grid : " << std::to_string(P.dP_Grid) << endl;
        output << "PV : " << std::to_string(P.dP_PV) << endl;
        output << "Day : " << std::to_string(P.iE_Day) << endl;
        output << "Year : " << std::to_string(P.dE_Year) << endl;
        output << "Total : " << std::to_string(P.dE_Total) << endl;
        return output;
    }
};

class InverterInfo
{

    std::map<const int, const std::string> m_statusMap{
        {0, "Startup"},
        {1, "Startup"},
        {2, "Startup"},
        {3, "Startup"},
        {4, "Startup"},
        {5, "Startup"},
        {6, "Startup"},
        {7, "Running"},
        {8, "Standby"},
        {9, "Bootloading"},
        {10, "Error"},
        {11, "Idle"},
        {12, "Ready"},
        {13, "Sleeping"},
        {255, "Unknown"},
    };

public:
    enum class Status
    {
        Startup,
        Running = 7,
        Standby,
        Bootloading,
        Error,
        Idle,
        Ready,
        Sleeping,
        Unknown = 255
    };

    std::string m_name{};
    int64_t m_deviceType{};
    unsigned int m_show{};
    std::string m_uniqueId;
    int64_t m_statusCode{};
    std::string m_statusCodeText{};
    int64_t m_errorCode{};
    int64_t m_PVPower{};

    friend ostream &operator<<(ostream &output, const InverterInfo &I)
    {
        output << "Name: " << I.m_name << endl;
        output << "DeviceType: " << I.m_deviceType << endl;
        output << "Show: " << I.m_show << endl;
        output << "UniqueId: " << I.m_uniqueId << endl;
        output << "StatusCode: " << I.m_statusCode << endl;
        output << "StatusCodeText: " << I.getStatusText(I.m_statusCode) << endl;
        output << "ErrorCode: " << I.m_errorCode << endl;
        output << "PV Power: " << I.m_PVPower << endl;
        return output;
    }

    const std::string &getStatusText(const int64_t &status) const
    {
        return this->m_statusMap.at(status);
    };

    const std::string convertHtmlToString(std::string const &input)
    {
        std::string output{};
        stringstream ss{input};
        std::string tmp{};
        while (getline(ss, tmp, ';'))
        {
            output += (char)std::stoi(tmp.substr(2));
        }
        return output;
    }

    /* const std::string convertHtmlToString(std::string const &input)
    {
        std::string output{};
        auto it = input.begin();
        auto it_pre_delim = std::find(it, input.end(), '#');
        auto it_post_delim = std::find(it, input.end(), ';');
        for (; it_post_delim != input.end(); )
        {
            output += (char)std::stoi(input.substr(std::distance(input.begin(), it_pre_delim + 1), std::distance(it_pre_delim + 1, it_post_delim)));
            it = it_post_delim + 1;
            it_pre_delim = std::find(it, input.end(), '#');
            it_post_delim = std::find(it, input.end(), ';');
        }
        return output;
    } */
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
    InverterInfo m_inverterInfo;
    PowerFlow m_powerFlow;

    FroniusClient(std::shared_ptr<HTTPClientSession> session);

public:
    static FroniusClient *create(std::shared_ptr<YAML::Node> parentNode, std::shared_ptr<HTTPClientSession> session);
    bool getApiVersion();
    void getPowerFlow();
    void getInverterInfo();
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

    const InverterInfo &getInverterInfoData()
    {
        m_inverterInfo.m_name = m_inverterInfo.convertHtmlToString(m_inverterInfo.m_name);
        m_inverterInfo.m_statusCodeText = m_inverterInfo.getStatusText(m_inverterInfo.m_statusCode);
        return m_inverterInfo;
    }

    ~FroniusClient();
};
