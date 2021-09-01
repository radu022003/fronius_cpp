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

bool FroniusClient::getApiVersion()
{
    bool result{false};
    try
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
        std::cout << "response: " << res.getStatus() << " " << res.getReason() << endl;

        std::string response{};
        StreamCopier::copyToString(is, response);

        Document d;

        d.Parse(response.c_str());
        if (d.Parse(response.c_str()).HasParseError())
        {
            std::cout << "error parsing response" << endl;
            std::cout << response << endl;
            return false;
        }
        assert(d.HasMember("BaseURL"));
        assert(d["BaseURL"].IsString());
        m_baseURL = d["BaseURL"].GetString();
        assert(d.HasMember("APIVersion"));
        assert(d["APIVersion"].IsInt());
        m_apiVersion = d["APIVersion"].GetInt();
        result = true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        result = false;
    }
    return result;
}

bool FroniusClient::getPowerFlow()
{
    bool result{false};
    try
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
            return false;
        }
        m_powerFlow = {
            0.0,
            0.0,
            0.0,
            0,
            0.0,
            0.0,
        };

        if (Value *type1 = GetValueByPointer((*d), "/Body/Data/Site"))
        {
            std::cout << "getting data from json" << std::endl;
            if (!(*type1)["P_Load"].IsNull())
            {
                m_powerFlow.dP_Load = (*type1)["P_Load"].GetDouble();
            }

            if (!(*type1)["P_Grid"].IsNull())
            {
                m_powerFlow.dP_Grid = (*type1)["P_Grid"].GetDouble();
            }

            if (!(*type1)["P_PV"].IsNull())
            {
                m_powerFlow.dP_PV = (*type1)["P_PV"].GetDouble();
            }

            if (!(*type1)["E_Day"].IsNull())
            {
                m_powerFlow.iE_Day = (*type1)["E_Day"].GetInt();
            }

            if (!(*type1)["E_Total"].IsNull())
            {
                m_powerFlow.dE_Total = (*type1)["E_Total"].GetDouble();
            }

            if (!(*type1)["E_Year"].IsNull())
            {
                m_powerFlow.dE_Year = (*type1)["E_Year"].GetDouble();
            }
        }
        result = true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "exception during GetPowerFlowRealtimeData: " << e.what() << '\n';
        result = false;
    }
    return result;
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
