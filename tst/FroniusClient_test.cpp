#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <read_config.h>
#include <fronius.h>
#include <fstream>
#include "test_data.h"

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

    EXPECT_TRUE(inverter->getApiVersion());

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
    EXPECT_CALL(*session, receiveResponse).WillOnce(ReturnRef(GetAPIVersion));
    EXPECT_TRUE(inverter->getApiVersion());

    EXPECT_EQ(inverter->getBaseURL(), "/solar_api/v1/");
    EXPECT_EQ(inverter->getApiVersionNumber(), 1);

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

TEST(test_Inverter2, get_powerflow_data)
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
    EXPECT_CALL(*session, receiveResponse).WillOnce(ReturnRef(FroniusHybridSys_GetPowerFlowRealtimeData_integer_values));

    inverter->getPowerFlow();

    const auto &power = inverter->getFlowPowerData();

    EXPECT_DOUBLE_EQ(power.dP_Load, -57);
    EXPECT_DOUBLE_EQ(power.dP_Grid, -975);
    EXPECT_DOUBLE_EQ(power.dP_PV, 1033);
    EXPECT_EQ(power.iE_Day, 15039);
    EXPECT_DOUBLE_EQ(power.dE_Total, 2777190);
    EXPECT_DOUBLE_EQ(power.dE_Year, 2777198);

    EXPECT_TRUE(testing::Mock::VerifyAndClearExpectations(session.get()));
    //delete shared pointer
    session.~__shared_ptr();
}

TEST(test_Inverter2, get_powerflow_data_null_values)
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
    EXPECT_CALL(*session, receiveResponse).WillOnce(ReturnRef(FroniusHybridSys_GetPowerFlowRealtimeData_null_values));

    inverter->getPowerFlow();

    const auto &power = inverter->getFlowPowerData();

    EXPECT_DOUBLE_EQ(power.dP_Load, 0);
    EXPECT_DOUBLE_EQ(power.dP_Grid, 0);
    EXPECT_DOUBLE_EQ(power.dP_PV, 0);
    EXPECT_EQ(power.iE_Day, 0);
    EXPECT_DOUBLE_EQ(power.dE_Total, 0);
    EXPECT_DOUBLE_EQ(power.dE_Year, 0);

    EXPECT_TRUE(testing::Mock::VerifyAndClearExpectations(session.get()));
    //delete shared pointer
    session.~__shared_ptr();
}

TEST(test_Inverter2, get_inverterInfo)
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
    EXPECT_CALL(*session, receiveResponse).WillOnce(ReturnRef(FroniusHybridSys_InverterInfo));

    inverter->getInverterInfo();

    const auto &inverterData = inverter->getInverterInfoData();

    std::cout << inverterData << endl;
    EXPECT_EQ(inverterData.m_name, "Primo 3.0-1 (1)");
    //EXPECT_EQ(inverterData.m_name, "&#80;&#114;&#105;&#109;&#111;&#32;&#51;&#46;&#48;&#45;&#49;&#32;&#40;&#49;&#41;");
    EXPECT_EQ(inverterData.m_deviceType, 81);
    EXPECT_EQ(inverterData.m_errorCode, 0);
    EXPECT_EQ(inverterData.m_show, 1);
    EXPECT_EQ(inverterData.m_statusCode, static_cast<int64_t>(InverterInfo::Status::Running));
    EXPECT_EQ(inverterData.m_statusCodeText, "Running");
    EXPECT_EQ(inverterData.m_uniqueId, "1148686");

    EXPECT_TRUE(testing::Mock::VerifyAndClearExpectations(session.get()));
    //delete shared pointer
    session.~__shared_ptr();
}

TEST(test_html_converter, convert_html_text)
{
    auto inverterInfo = new InverterInfo();
    std::string input{"&#80;&#114;&#105;&#109;&#111;&#32;&#51;&#46;&#48;&#45;&#49;&#32;&#40;&#49;&#41;"};
    EXPECT_EQ(inverterInfo->convertHtmlToString(input), "Primo 3.0-1 (1)");
}
