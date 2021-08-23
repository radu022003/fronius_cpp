#include <fronius.h>
#include <read_config.h>

int main()
{
	std::cout << "Fronius client app started\nLooking for config.yaml file" << std::endl;
	auto config = ReadConfig::create();
	std::shared_ptr<YAML::Node> parentNode = config->parseConfig("config.yaml");
	auto inverter = FroniusClient::create(parentNode);

	return 0;
}