#include <iostream>

#include "AggregationConfig.h"
#include "AggregationSettings.h"
#include "AcquisitionServersConfig.h"
#include "DatabaseConfig.h"
#include "DBStorage.h"
#include "AcquisitionScheduler.h"
#include "../../common/Utils.h"
#include "../../common/data/Station.h"

using namespace Stormy;
using namespace std;

int main() {
	cout << "==== Storage & Aggregation started. ====" << endl;
	AcquisitionServersConfig acquisitionServersCfg("config/acquisition_servers.yaml");
	AggregationConfig aggregationCfg("config/aggregation.yaml");
	DatabaseConfig storageDBcfg("config/storage_database.yaml");
	DatabaseConfig aggregationDBcfg("config/aggregation_database.yaml");

	DBStorage storage(storageDBcfg.getConfiguration());
	cout << "Measurements in storage: " << storage.countAllMeasurements() << endl;

	// display current configurations
	Utils::forEach(acquisitionServersCfg.getConfiguration(),
		[](AcquisitionServer* server) {
			cout << server -> toString() << endl;
	});
	Utils::forEach(aggregationCfg.getConfiguration(),
		[](AggregationSettings setting) {
			cout << setting.toString() << endl;
	});
	cout << storageDBcfg.getConfiguration()->toString() << endl;
	AcquistionScheduler scheduler(&storage, acquisitionServersCfg.getConfiguration());

	getchar();
}