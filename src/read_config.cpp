#include <read_config.h>
#include <iostream>
#include <yaml-cpp/yaml.h>

ReadConfig::ReadConfig(/* args */)
{
    //check if config file already exists
    YAML::Node config = YAML::LoadFile("config.yaml");
    std::cout << "creating ReadConfig instance" << std::endl;
}

ReadConfig::~ReadConfig()
{
}

ReadConfig *ReadConfig::create()
{
    return new ReadConfig();
}