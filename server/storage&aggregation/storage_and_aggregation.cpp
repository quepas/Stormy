#include <iostream>

#include "AggregationConfig.h"
#include "AggregationSettings.h"
#include "AcquisitionServersConfig.h"
#include "DatabaseConfig.h"
#include "DBStorage.h"
#include "DBAggregation.h"
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
	DBAggregation aggregation(aggregationDBcfg.getConfiguration(), &storage);
	cout << "Measurements in storage: " << storage.countAllMeasurements() << endl;

	cout << "-------------------------------------------------------------"
		"-------------------------------------------------------------"<< endl;
	// display current configurations
	cout << "=== Acquisition Servers: " << endl;
	Utils::forEach(acquisitionServersCfg.getConfiguration(),
		[](AcquisitionServer* server) {
			cout << "\t" << server -> toString() << endl;
	});
	cout << "=== Available aggregates: " << endl;
	Utils::forEach(aggregationCfg.getConfiguration(),
		[](AggregationSettings setting) {
			cout << "\t" << setting.toString() << endl;
	});
	cout << "=== Storage database: " << endl;
	cout << "\t" << storageDBcfg.getConfiguration()->toString() << endl;
	cout << "=== Aggregation database: " << endl;
	cout << "\t" << aggregationDBcfg.getConfiguration()->toString() << endl;
	cout << "-------------------------------------------------------------"
		"-------------------------------------------------------------"<< endl;
	aggregation.insertPeriods(aggregationCfg.getConfiguration());
	AcquistionScheduler scheduler(&storage, acquisitionServersCfg.getConfiguration());

	getchar();
}