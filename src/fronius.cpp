#include <fronius.h>

FroniusClient::FroniusClient(std::shared_ptr<HTTPClientSession> session) : m_apiVersion(0)
{
    m_session = std::move(session);
    m_uri.setHost(m_session->getHost());
    m_uri.setPort(m_session->getPort());
}

FroniusClient::~FroniusClient()
{
}

FroniusClient *FroniusClient::create(std::shared_ptr<YAML::Node> parentNode, std::shared_ptr<HTTPClientSession> session)
{
    URI uri("http://" + (*parentNode)["host"].as<std::string>());
    session->setHost(uri.getHost());
    session->setPort(uri.getPort());

    return new FroniusClient(std::move(session));
}

void FroniusClient::getApiVersion()
{
    m_uri.setPath("/solar_api/GetAPIVersion.cgi");
    string path(m_uri.getPathAndQuery());
    // send request
    HTTPRequest req(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
    m_session->sendRequest(req);
    // get response
    HTTPResponse res;
    // print response
    istream &is = m_session->receiveResponse(res);
    cout << "response: " << res.getStatus() << " " << res.getReason() << endl;

    std::string response{};
    StreamCopier::copyToString(is, response);

    Document d;

    d.Parse(response.c_str());
    if (d.Parse(response.c_str()).HasParseError())
    {
        cout << "error parsing response" << endl;
        return;
    }
    assert(d.HasMember("BaseURL"));
    assert(d["BaseURL"].IsString());
    m_baseURL = d["BaseURL"].GetString();
    assert(d.HasMember("APIVersion"));
    assert(d["APIVersion"].IsInt());
    m_apiVersion = d["APIVersion"].GetInt();
}

void FroniusClient::getPowerFlow()
{
    m_uri.setPath(m_baseURL + "GetPowerFlowRealtimeData.fcgi");
    string path(m_uri.getPathAndQuery());
    cout << path << endl;
    // send request
    HTTPRequest req(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
    m_session->sendRequest(req);
    // get response
    HTTPResponse res;
    // print response
    istream &is = m_session->receiveResponse(res);
    cout << "response: " << res.getStatus() << " " << res.getReason() << endl;

    std::string response{};
    StreamCopier::copyToString(is, response);
    std::cout << "powerFlow realtime data response: " << response << std::endl;
    auto d = std::make_unique<Document>();

    d->Parse(response.c_str());
    if (d->Parse(response.c_str()).HasParseError())
    {
        cout << "parsing error: " << GetParseError_En(d->GetParseError()) << endl;
        cout << "error parsing response" << endl;
        return;
    }

    if (Value *type1 = GetValueByPointer((*d), "/Body/Data/Site"))
    {
        if (!(*type1)["P_Load"].IsNull())
        {
            m_powerFlow.dP_Load = (*type1)["P_Load"].GetDouble();
        }
        else
        {
            m_powerFlow.dP_Load = 0.0;
        }

        if (!(*type1)["P_Grid"].IsNull())
        {
            m_powerFlow.dP_Grid = (*type1)["P_Grid"].GetDouble();
        }
        else
        {
            m_powerFlow.dP_Grid = 0.0;
        }

        if (!(*type1)["P_PV"].IsNull())
        {
            m_powerFlow.dP_PV = (*type1)["P_PV"].GetDouble();
        }
        else
        {
            m_powerFlow.dP_PV = 0.0;
        }

        if (!(*type1)["E_Day"].IsNull())
        {
            m_powerFlow.iE_Day = (*type1)["E_Day"].GetInt();
        }
        else
        {
            m_powerFlow.iE_Day = 0;
        }
        if (!(*type1)["E_Total"].IsNull())
        {
            m_powerFlow.dE_Total = (*type1)["E_Total"].GetDouble();
        }
        else
        {
            m_powerFlow.dE_Total = 0.0;
        }
        if (!(*type1)["E_Year"].IsNull())
        {
            m_powerFlow.dE_Year = (*type1)["E_Year"].GetDouble();
        }
        else
        {
            m_powerFlow.dE_Year = 0.0;
        }
    }
}

void FroniusClient::getInverterInfo()
{
    m_uri.setPath(m_baseURL + "GetInverterInfo.cgi");
    string path(m_uri.getPathAndQuery());
    cout << path << endl;
    // send request
    HTTPRequest req(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
    m_session->sendRequest(req);
    // get response
    HTTPResponse res;
    // print response
    istream &is = m_session->receiveResponse(res);
    cout << "response: " << res.getStatus() << " " << res.getReason() << endl;

    std::string response{};
    StreamCopier::copyToString(is, response);
    std::cout << "powerFlow realtime data response: " << response << std::endl;
    auto d = std::make_unique<Document>();

    d->Parse(response.c_str());
    if (d->Parse(response.c_str()).HasParseError())
    {
        cout << "parsing error: " << GetParseError_En(d->GetParseError()) << endl;
        cout << "error parsing response" << endl;
        return;
    }

    if (Value *type1 = GetValueByPointer((*d), "/Body/Data/1"))
    {
        m_inverterInfo.m_name = (*type1)["CustomName"].GetString();
        m_inverterInfo.m_deviceType = (*type1)["DT"].GetInt();
        m_inverterInfo.m_errorCode = (*type1)["ErrorCode"].GetInt();
        m_inverterInfo.m_PVPower = (*type1)["PVPower"].GetInt();
        m_inverterInfo.m_show = (*type1)["Show"].GetInt();
        m_inverterInfo.m_statusCode = (*type1)["StatusCode"].GetInt();
        m_inverterInfo.m_uniqueId = (*type1)["UniqueID"].GetString();
    }
}
