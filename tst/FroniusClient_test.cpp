#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <read_config.h>
#include <fronius.h>
#include <fstream>

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;
using namespace Poco::Net;
using namespace Poco;
using namespace std;

class HTTPClientSessionMock : public HTTPClientSession
{
private:
    /* data */
    const std::string resp{"ok"};

public:
    HTTPClientSessionMock(/* args */){};
    virtual ~HTTPClientSessionMock(){};
    MOCK_METHOD1(receiveResponse, std::istream &(HTTPResponse &res));
    MOCK_METHOD1(sendRequest, std::ostream &(HTTPRequest &request));
};

TEST(yaml_config, create_file_with_default_values_if_none_exists)
{
    //arrange
    //act
    //assert
    auto config = ReadConfig::create();
    auto parentNode = config->parseConfig("default_config.yaml");
    EXPECT_NE(config, nullptr);

    ASSERT_EQ((*parentNode)["database"]["user"].as<std::string>(), "admin");
    ASSERT_EQ((*parentNode)["database"]["pass"].as<std::string>(), "admin");
    ASSERT_EQ((*parentNode)["database"]["ip"].as<std::string>(), "localhost");

    ASSERT_EQ((*parentNode)["freq"].as<std::string>(), "10");
    ASSERT_EQ((*parentNode)["host"].as<std::string>(), "localhost:80");
}

TEST(yaml_config, create_fronius_client_from_config)
{
    //arrange
    //act
    //assert
    auto config = ReadConfig::create();
    std::shared_ptr<YAML::Node> parentNode = config->parseConfig("config.yaml");
    auto session = std::make_shared<HTTPClientSessionMock>();

    auto inverter = FroniusClient::create(parentNode, session);

    EXPECT_EQ(inverter->getHost(), "localhost");
    EXPECT_EQ(inverter->getPort(), 80);

    EXPECT_NE(config, nullptr);
    EXPECT_NE(inverter, nullptr);

    std::istringstream str(R"!(
{
    "APIVersion" : 1,
    "BaseURL" : "/solar_api/v1/",
    "CompatibilityRange" : "1.5-18"
}
    )!");
    std::ostream &os = std::cout;
    EXPECT_CALL(*session, sendRequest).WillOnce(ReturnRef(os));
    EXPECT_CALL(*session, receiveResponse).WillOnce(ReturnRef(str));

    inverter->getApiVersion();
    EXPECT_TRUE(testing::Mock::VerifyAndClearExpectations(session.get()));
    //delete shared pointer
    session.~__shared_ptr();
}
