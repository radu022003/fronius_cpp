#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <read_config.h>
#include <fronius.h>
#include <fstream>
#include "test_data.h"
#include <sqlite_conn.h>

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
}

TEST(test_Inverter, get_api_version_from_inverter)
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
    EXPECT_EQ(inverter->getBaseURL(), "");
    EXPECT_EQ(inverter->getApiVersionNumber(), 0);

    inverter->getApiVersion();

    EXPECT_EQ(inverter->getBaseURL(), "/solar_api/v1/");
    EXPECT_EQ(inverter->getApiVersionNumber(), 1);

    EXPECT_TRUE(testing::Mock::VerifyAndClearExpectations(session.get()));
    //delete shared pointer
    session.~__shared_ptr();
}

TEST(test_Inverter2, get_api_version_from_inverter)
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

    std::ostream &os = std::cout;
    EXPECT_CALL(*session, sendRequest).WillOnce(ReturnRef(os));
    EXPECT_CALL(*session, receiveResponse).WillOnce(ReturnRef(FroniusHybridSys_GetPowerFlowRealtimeData));

    inverter->getPowerFlow();

    const auto &power = inverter->getFlowPowerData();

    EXPECT_DOUBLE_EQ(power.dP_Load, 5.9900000000000091);
    EXPECT_DOUBLE_EQ(power.dP_Grid, -511.99000000000001);
    EXPECT_DOUBLE_EQ(power.dP_PV, 941.60000000000002);
    EXPECT_EQ(power.iE_Day, 6758);
    EXPECT_DOUBLE_EQ(power.dE_Total, 7604385.5);
    EXPECT_DOUBLE_EQ(power.dE_Year, 1342638.2000000002);

    EXPECT_TRUE(testing::Mock::VerifyAndClearExpectations(session.get()));
    //delete shared pointer
    session.~__shared_ptr();
}

TEST(test_Inverter2, get_api_version_from_inverter2)
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

    std::ostream &os = std::cout;
    EXPECT_CALL(*session, sendRequest).WillOnce(ReturnRef(os));
    EXPECT_CALL(*session, receiveResponse).WillOnce(ReturnRef(FroniusHybridSys_GetPowerFlowRealtimeData2));

    inverter->getPowerFlow();

    const auto &power = inverter->getFlowPowerData();

    EXPECT_DOUBLE_EQ(power.dP_Load, -57.649999999999977);
    EXPECT_DOUBLE_EQ(power.dP_Grid, -975.35000000000002);
    EXPECT_DOUBLE_EQ(power.dP_PV, 1033);
    EXPECT_EQ(power.iE_Day, 15039);
    EXPECT_DOUBLE_EQ(power.dE_Total, 2777190);
    EXPECT_DOUBLE_EQ(power.dE_Year, 2777198.75);

    EXPECT_TRUE(testing::Mock::VerifyAndClearExpectations(session.get()));
    //delete shared pointer
    session.~__shared_ptr();
}

TEST(test_sqlite, create_table)
{
    //arrange
    //act
    //assert
    auto db = SqliteConn::create("sample.db");
    PowerFlow sampleData = {
        5.9900000000000091,
        -511.99000000000001,
        941.60000000000002,
        6758,
        7604385.5,
        1342638.2000000002};
}

TEST(test_sqlite, insert_values)
{
    //arrange
    //act
    //assert

    PowerFlow sampleData = {
        5.9900000000000091,
        -511.99000000000001,
        941.60000000000002,
        6758,
        7604385.5,
        1342638.2000000002};
    auto db = SqliteConn::create("sample2.db");
    db->insert(sampleData);
}