#include <read_config.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

ReadConfig::ReadConfig(/* args */)
{
    //check if config file already exists
    try
    {
        YAML::Node config = YAML::LoadFile("config.yaml");
    }
    catch (const std::exception &e)
    {
        std::cerr << "exception opening config file, " << e.what() << '\n';
        std::istringstream istream(e.what());
        if (istream.str().find("bad file") != std::string::npos)
        {
            std::cout << "no config file exists, a default file will be created" << std::endl;
            std::ofstream out_config("config.yaml", std::ofstream::out);
            out_config << ReadConfig::create_default_data();
        }
        else if (istream.str().find("error"))
        {
            std::cout << "config file is corrupted, please correct it" << std::endl;
        }
    }

    std::cout << "creating ReadConfig instance" << std::endl;
}

ReadConfig::~ReadConfig()
{
}

ReadConfig *ReadConfig::create()
{
    return new ReadConfig();
}

YAML::Node ReadConfig::create_default_data()
{
    YAML::Node node;
    YAML::Node dbNode = YAML::Load("{user: admin, pass: admin, ip: localhost}");
    node["database"] = dbNode;
    node["freq"] = 10;
    node["ip_address"] = "192.168.100.116";
    return node;
}

std::unique_ptr<YAML::Node> ReadConfig::parseConfig()
{
    YAML::Node node = YAML::LoadFile("config.yaml");
    return std::make_unique<YAML::Node>(node);
}