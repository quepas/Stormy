#include <iostream>

#include "AcquisitionServersConfig.h"
#include "../../common/Utils.h"

using namespace Stormy;

int main() {
	std::cout << "==== Storage & Aggregation is running... ====" << std::endl;
	AcquisitionServersConfig cfg("config/acquisition_servers.yaml");
	Utils::forEach(cfg.getConfiguration(), [](AcquisitionServer* server) {
		std::cout << server->formatData() << std::endl;
	});

	getchar();
}