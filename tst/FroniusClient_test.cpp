#include "gtest/gtest.h"
#include <read_config.h>
#include <fronius.h>

TEST(blaTest, test1)
{
    //arrange
    //act
    //assert
    EXPECT_EQ(0, 0);
}

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
    auto inverter = FroniusClient::create(parentNode);
    EXPECT_NE(config, nullptr);
}
