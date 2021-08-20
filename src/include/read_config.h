#pragma once
#include <yaml-cpp/yaml.h>
#include <memory>

class ReadConfig
{
private:
    /* data */
    ReadConfig(/* args */);
    YAML::Node create_default_data();
    std::unique_ptr<YAML::Node> parentNode;

public:
    static ReadConfig *create();
    std::unique_ptr<YAML::Node> parseConfig();
    ~ReadConfig();
};
