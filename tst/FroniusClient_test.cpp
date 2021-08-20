#include "gtest/gtest.h"
#include <read_config.h>

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
    auto parentNode = config->parseConfig();
    EXPECT_NE(config, nullptr);

    ASSERT_EQ((*parentNode)["database"]["user"].as<std::string>(), "admin");
    ASSERT_EQ((*parentNode)["database"]["pass"].as<std::string>(), "admin");
    ASSERT_EQ((*parentNode)["database"]["ip"].as<std::string>(), "localhost");

    ASSERT_EQ((*parentNode)["freq"].as<std::string>(), "10");
    ASSERT_EQ((*parentNode)["ip_address"].as<std::string>(), "192.168.100.116");
}