#include <fronius.h>

FroniusClient::FroniusClient(std::shared_ptr<HTTPClientSession> session)
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
    StreamCopier::copyStream(is, cout);
}