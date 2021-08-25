#include <fronius.h>

FroniusClient::FroniusClient(std::shared_ptr<HTTPClientSession> session) : m_apiVersion(0)
{
    //std::cout << "session host in constr: " << session->getHost() << endl;
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
    //std::cout << "session host in create: " << session->getHost() << endl;
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
    //StreamCopier::copyStream(is, cout);
    std::string response{};
    StreamCopier::copyToString(is, response);
    //cout << response << endl;
    //const char *json = "{\"project\":\"Your_Project_name\",\"Num_stars\":10}";
    Document d;
    //assert(d.IsObject());
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
    //cout << response << endl;

    auto d = std::make_unique<Document>();
    //Document d;
    //assert(d.IsObject());
    d->Parse(response.c_str());
    if (d->Parse(response.c_str()).HasParseError())
    {
        cout << "parsing error: " << GetParseError_En(d->GetParseError()) << endl;
        cout << "error parsing response" << endl;
        return;
    }

    assert((*d)["Body"]["Data"]["Site"].HasMember("P_Load"));
    assert((*d)["Body"]["Data"]["Site"]["P_Load"].IsDouble());
    m_powerFlow.dP_Load = (*d)["Body"]["Data"]["Site"]["P_Load"].GetDouble();

    if (Value *type1 = GetValueByPointer((*d), "/Body/Data/Site"))
    {
        m_powerFlow.dP_Grid = (*type1)["P_Grid"].GetDouble();
        m_powerFlow.dP_PV = (*type1)["P_PV"].GetDouble();
        m_powerFlow.iE_Day = (*type1)["E_Day"].GetInt();
        m_powerFlow.dE_Total = (*type1)["E_Total"].GetDouble();
        m_powerFlow.dE_Year = (*type1)["E_Year"].GetDouble();
    }
}