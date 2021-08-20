#include "gtest/gtest.h"
//#include "Formula.h"
#include <read_config.h>

TEST(blaTest, test1)
{
    //arrange
    //act
    //assert
    EXPECT_EQ(0, 0);
}

TEST(yaml_config, open_file)
{
    //arrange
    //act
    //assert
    auto config = ReadConfig::create();
    EXPECT_NE(config, nullptr);
    EXPECT_EQ(0, 0);
}