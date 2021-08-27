#include <fronius.h>
#include <read_config.h>
#include <sqlite_conn.h>

int main()
{
	std::cout << "Fronius client app started\nLooking for config.yaml file" << std::endl;
	auto config = ReadConfig::create();
	std::shared_ptr<YAML::Node> parentNode = config->parseConfig("config.yaml");
	auto session = std::make_unique<HTTPClientSession>();
	URI uri((*parentNode)["host"].as<std::string>());
	session->setHost(uri.getHost());
	session->setPort(uri.getPort());
	cout << "create instance" << endl;
	auto inverter = FroniusClient::create(parentNode, std::move(session));
	cout << "get apiVersion" << endl;
	inverter->getApiVersion();
	auto db = SqliteConn::create("fronius.db");
	while (true)
	{
		cout << "get FlowPowerData" << endl;
		const auto &power = inverter->getFlowPowerData();
		cout << "print FlowPowerData" << endl;
		cout << power;
		sleep(1);
		db->insert(const_cast<PowerFlow &>(power));
	}

	return 0;
}
