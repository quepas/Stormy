#include <iostream>

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
	DatabaseConfig storageDBcfg("config/storage_database.yaml");
	DBStorage storage(storageDBcfg.getConfiguration());		
	cout << "Measurements in storage: " << storage.countAllMeasurements() << endl;

	// display current configurations
	Utils::forEach(acquisitionServersCfg.getConfiguration(), 
		[](AcquisitionServer* server) {
			cout << server -> toString() << endl;
	});
	cout << storageDBcfg.getConfiguration()->toString() << endl;	
	AcquistionScheduler scheduler(&storage, acquisitionServersCfg.getConfiguration());

	getchar();
}