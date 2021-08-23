#include <fronius.h>

FroniusClient::FroniusClient(std::shared_ptr<YAML::Node> parentNode)
{
    m_uri = "http://" + (*parentNode)["host"].as<std::string>();
    m_session.setHost(m_uri.getHost());
    m_session.setPort(m_uri.getPort());
}

FroniusClient::~FroniusClient()
{
}

FroniusClient *FroniusClient::create(std::shared_ptr<YAML::Node> parentNode)
{
    return new FroniusClient(parentNode);
}